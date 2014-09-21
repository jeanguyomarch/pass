#include "pass.h"

static char *_file = NULL;
static Eet_File *_ef = NULL;

/*============================================================================*
 *                                   Helpers                                  *
 *============================================================================*/

static inline Eina_Bool
_file_open_ro(void)
{
   _ef = eet_open(_file, EET_FILE_MODE_READ);
   return !!_ef;
}

static inline Eina_Bool
_file_open_rw(void)
{
   _ef = eet_open(_file, EET_FILE_MODE_READ_WRITE);
   return !!_ef;
}

static inline void
_file_close(void)
{
   eet_close(_ef);
   _ef = NULL;
}


/*============================================================================*
 *                                Init/Shutdown                               *
 *============================================================================*/

Eina_Bool
file_init(void)
{
   char buf[PATH_MAX];
   int len;

   len = snprintf(buf, sizeof(buf), "%s/.pass_db", getenv("HOME"));
   _file = strndup(buf, len);
   EINA_SAFETY_ON_NULL_GOTO(_file, path_err);

   return EINA_TRUE;

path_err:
   return EINA_FALSE;
}

void
file_shutdown(void)
{
   free(_file);
   if (_ef)
     {
        ERR("It seems Eet_File was not closed. I'll do it for you...");
        _file_close();
     }
}

int
file_list(void)
{
   int count;
   Eet_Entry *entry;
   Eina_Iterator *entries;

   if (!_file_open_ro())
     {
        INF("No entry available");
        return 1;
     }

   count = eet_num_entries(_ef);
   entries = eet_list_entries(_ef);

   EINA_ITERATOR_FOREACH(entries, entry)
      output("%s\n", entry->name);
   eina_iterator_free(entries);

   _file_close();
   return 0;
}

int
file_add(const char *key,
         const char *data,
         const char *cipher)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(key, 2);
   EINA_SAFETY_ON_NULL_RETURN_VAL(data, 2);
   EINA_SAFETY_ON_NULL_RETURN_VAL(cipher, 2);

   int size;

   if (!_file_open_rw())
     {
        CRI("Failed to open file %s", _file);
        return 1;
     }

   size = eet_write_cipher(_ef, key, data, strlen(data), 1, cipher);
   if (size == 0)
     {
        CRI("Failed to register entry \"%s\"", key);
        goto stop;
     }
   else
     INF("Save data for entry \"%s\"", key);

   _file_close();
   return 0;

stop:
   _file_close();
   return 1;
}

int
file_del(const char *key)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(key, 2);

   int chk;

   _file_open_rw();
   chk = eet_delete(_ef, key);
   _file_close();

   if (chk == 0)
     {
        CRI("Failed to delete entry \"%s\"", key);
        return 2;
     }

   return 0;
}

char *
file_get(const char *key,
         const char *cipher)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(key, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(cipher, NULL);

   void *data;
   int size;

   _file_open_ro();
   data = eet_read_cipher(_ef, key, &size, cipher);
   _file_close();

   return data;
}

Eina_Bool
file_entry_exists(const char *key)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(key, EINA_FALSE);

   void *handle;
   int size;

   _file_open_ro();
   handle = eet_read(_ef, key, &size);
   _file_close();
   free(handle);

   return (handle != NULL);
}

