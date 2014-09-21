#include "pass.h"

int _pass_log_dom = -1;
Clipboard_Set_Func clipboard_set = NULL;
Output_Func output = NULL;

/*============================================================================*
 *                              Logging Callbacks                             *
 *============================================================================*/

static void
_stdout(const char *format, ...)
{
   va_list args;

   va_start(args, format);
   vfprintf(stdout, format, args);
   va_end(args);
}

/*============================================================================*
 *                                Init/Shutdown                               *
 *============================================================================*/

static inline Eina_Bool
_init(void)
{
   int chk;

   /* Set logger */
   output = _stdout;
   EINA_SAFETY_ON_NULL_RETURN_VAL(output, EINA_FALSE);

   chk = eina_init();
   EINA_SAFETY_ON_TRUE_GOTO(chk <= 0, eina_err);
   chk = eet_init();
   EINA_SAFETY_ON_TRUE_GOTO(chk <= 0, eet_err);

   _pass_log_dom = eina_log_domain_register("pass", EINA_COLOR_RED);
   EINA_SAFETY_ON_TRUE_GOTO(_pass_log_dom < 0, log_err);

   chk = clipboard_init();
   EINA_SAFETY_ON_FALSE_GOTO(chk, clipboard_err);

   chk = file_init();
   EINA_SAFETY_ON_FALSE_GOTO(chk, file_err);

   chk = tty_init();
   EINA_SAFETY_ON_FALSE_GOTO(chk, tty_err);

   return EINA_TRUE;

tty_err:
   file_shutdown();
file_err:
   clipboard_shutdown();
clipboard_err:
   eina_log_domain_unregister(_pass_log_dom);
log_err:
   eet_shutdown();
eet_err:
   eina_shutdown();
eina_err:
   return EINA_FALSE;
}

static inline void
_shutdown(void)
{
   tty_shutdown();
   file_shutdown();
   clipboard_shutdown();
   eina_log_domain_unregister(_pass_log_dom);
   eet_shutdown();
   eina_shutdown();
}

/*============================================================================*
 *                                   Getopt                                   *
 *============================================================================*/

static const Ecore_Getopt _options =
{
   "pass",
   "%prog [options] (at least one)",
   "1.0",
   "(C) 2013-2014 Jean Guyomarc'h",
   "MIT",
   "A command-line password manager",
   EINA_TRUE,
   {
      ECORE_GETOPT_STORE_TRUE('l', "list", "Display the list of stored data"),
      ECORE_GETOPT_STORE_STR('a', "add", "Add a new string to the encrypted database"),
      ECORE_GETOPT_STORE_STR('d', "delete", "Deletes the entry for the given key"),
      ECORE_GETOPT_STORE_STR('x', "extract", "Extracts the entry for the given key"),
      ECORE_GETOPT_STORE_TRUE('g', "generate", "Generates a random password in your clipboard using openSSL"),
      ECORE_GETOPT_STORE_STR('r', "replace", "Replace by the provided key"),
      ECORE_GETOPT_STORE_STR('R', "rename", "Rename the provided key"),
      ECORE_GETOPT_HELP ('h', "help"),
      ECORE_GETOPT_VERSION('V', "version"),
      ECORE_GETOPT_SENTINEL
   }
};

static inline void
_help_show(void)
{
   ecore_getopt_help(stderr, &_options);
}

/*============================================================================*
 *                                   Helpers                                  *
 *============================================================================*/

static int
_pass_add(const char *key)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(key, 2);

   char *password = NULL, *cipher = NULL;
   int status;

   if (file_entry_exists(key))
     {
        ERR("Entry \"%s\" already exists", key);
        return 2;
     }

   output("Write the data you want to store:\n");
   password = tty_string_silent_get(NULL);
   output("Write a cipher key to encrypt the data:\n");
   cipher = tty_string_silent_get(NULL);

   if (EINA_UNLIKELY(!password) || EINA_UNLIKELY(!cipher))
     {
        status = 2;
        goto end;
     }

   status = file_add(key, password, cipher);

end:
   free(password);
   free(cipher);

   return status;
}

static int
_pass_del(const char *key,
          const char *verb)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(key, 2);
   EINA_SAFETY_ON_NULL_RETURN_VAL(verb, 2);

   char *str;
   int status;

   if (!file_entry_exists(key))
     {
        ERR("Entry \"%s\" does not exist", key);
        return 1;
     }

   output("Are you sure to %s \"%s\". It cannot be recovered! [y/N] ",
          verb, key);
   str = tty_string_get(NULL);
   if ((str != NULL) && (str[0] == 'y' || str[0] == 'Y'))
     status = file_del(key);
   else
     status = 1;
   free(str);

   return status;
}

static int
_pass_extract(const char *key)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(key, 2);

   char *data, *cipher;
   Eina_Bool chk;

   if (!file_entry_exists(key))
     {
        ERR("Entry \"%s\" does not exist", key);
        return 1;
     }

   output("Write your decipher key:\n");
   cipher = tty_string_silent_get(NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(cipher, 2);

   data = file_get(key, cipher);
   chk = clipboard_set(data, -1);
   if (!chk)
     CRI("Failed to set data to clipboard");

   free(cipher);
   free(data);

   return (!chk); // 0 if chk is TRUE
}

static int
_pass_rename(const char *old_key,
             const char *new_key)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(new_key, 2);
   EINA_SAFETY_ON_NULL_RETURN_VAL(old_key, 2);

   if (!file_entry_exists(old_key))
     {
        ERR("Entry \"%s\" does not exist", old_key);
        return 1;
     }
   if (file_entry_exists(new_key))
     {
        ERR("Entry \"%s\" already exists", new_key);
        return 1;
     }

   return file_replace(old_key, new_key);
}


/*============================================================================*
 *                                    Main                                    *
 *============================================================================*/

int
main(int    argc,
     char **argv)
{
   Eina_Bool quit_opt = EINA_FALSE;
   Eina_Bool list_opt = EINA_FALSE;
   Eina_Bool gen_opt = EINA_FALSE;
   char *add_opt = NULL;
   char *del_opt = NULL;
   char *get_opt = NULL;
   char *replace_opt = NULL;
   char *rename_opt = NULL;
   int args;
   int status = EXIT_FAILURE;

   Ecore_Getopt_Value values[] = {
        ECORE_GETOPT_VALUE_BOOL(list_opt),
        ECORE_GETOPT_VALUE_STR(add_opt),
        ECORE_GETOPT_VALUE_STR(del_opt),
        ECORE_GETOPT_VALUE_STR(get_opt),
        ECORE_GETOPT_VALUE_BOOL(gen_opt),
        ECORE_GETOPT_VALUE_STR(replace_opt),
        ECORE_GETOPT_VALUE_STR(rename_opt),
        ECORE_GETOPT_VALUE_BOOL(quit_opt),
        ECORE_GETOPT_VALUE_BOOL(quit_opt),
        ECORE_GETOPT_VALUE_NONE
   };

   args = ecore_getopt_parse(&_options, values, argc, argv);
   if (args == 1)
     {
        _help_show();
        return EXIT_FAILURE;
     }
   else if (args < 0)
     return EXIT_FAILURE;

   if (quit_opt)
     return EXIT_SUCCESS;

   if (EINA_UNLIKELY(!_init()))
     return EXIT_FAILURE;

   /* List all entries */
   if (list_opt)
     {
        status = file_list();
        goto end;
     }

   /* Add an entry */
   if (add_opt)
     {
        if (replace_opt)
          {
             status = _pass_del(replace_opt, "replace");
             if (status != 0)
               {
                  ERR("Failed to remove key \"%s\"", replace_opt);
                  goto end;
               }
          }
        status = _pass_add(add_opt);
        goto end;
     }

   /* Delete an entry */
   if (del_opt)
     {
        status = _pass_del(del_opt, "delete");
        goto end;
     }

   /* Get the content of an entry */
   if (get_opt)
     {
        status = _pass_extract(get_opt);
        goto end;
     }

   /* Rename an entry */
   if (rename_opt)
     {
        if (!replace_opt)
          {
             ERR("This options must be used in pair with --replace");
             goto end;
          }
        status = _pass_rename(rename_opt, replace_opt);
        goto end;
     }

   /* Only replace option */
   if (replace_opt)
     {
        ERR("The --replace option must be used with the --add or --rename option\n");
        _help_show();
        goto end;
     }

end:
   _shutdown();

   return status;
}

