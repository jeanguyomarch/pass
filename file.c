#include "pass.h"

static char *_file = NULL;
static Eet_File *_ef = NULL;

#define COMPRESS 1 /* 1: YES; 0: NO */

/*============================================================================*
 *                                Init/Shutdown                               *
 *============================================================================*/

Eina_Bool
file_init(const char *file)
{
   char buf[PATH_MAX];
   int len;

   if (file == NULL)
     {
        len = snprintf(buf, sizeof(buf), "%s/.pass_db", getenv("HOME"));
        _file = strndup(buf, len);
     }
   else
     _file = strdup(file);

   EINA_SAFETY_ON_NULL_GOTO(_file, path_err);

   _ef = eet_open(_file, EET_FILE_MODE_READ_WRITE);
   EINA_SAFETY_ON_NULL_GOTO(_ef, file_err);

   return EINA_TRUE;

file_err:
   free(_file);
path_err:
   return EINA_FALSE;
}

void
file_shutdown(void)
{
   Eet_Error err;

   err = eet_sync(_ef);
   if (EINA_UNLIKELY(err != EET_ERROR_NONE))
     CRI("eet_sync() failed with error %i", err);
   err = eet_close(_ef);
   if (EINA_UNLIKELY(err != EET_ERROR_NONE))
     CRI("eet_close() failed with error %i", err);
   _ef = NULL;

   free(_file);
   _file = NULL;
}

int
file_list(void)
{
   int count;
   Eet_Entry *entry;
   Eina_Iterator *entries;

   count = eet_num_entries(_ef);
   entries = eet_list_entries(_ef);

   EINA_ITERATOR_FOREACH(entries, entry)
      output("%s\n", entry->name);
   eina_iterator_free(entries);

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

   size = eet_write_cipher(_ef, key, data, strlen(data), COMPRESS, cipher);
   if (size == 0)
     {
        CRI("Failed to register entry \"%s\"", key);
        return 1;
     }

   INF("Save data for entry \"%s\"", key);
   return 0;
}

int
file_del(const char *key)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(key, 2);

   int chk;

   chk = eet_delete(_ef, key);
   if (EINA_UNLIKELY(chk == 0))
     {
        CRI("Failed to delete entry \"%s\"", key);
        return 2;
     }

   INF("Delete data for entry \"%s\"", key);
   return 0;
}

char *
file_get(const char *key,
         const char *cipher)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(key, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(cipher, NULL);

   char *data;
   int size;

   data = eet_read_cipher(_ef, key, &size, cipher);
   INF("Data after extraction: \"%s\" (size: %i)", data, size);

   /* I really don't want newlines in the password! */
   size--;
   if (data[size] == '\n')
     data[size] = 0;

   return data;
}

int
file_replace(const char *old_key,
             const char *new_key)
{
   CRI("This operation is not implemented, sorry");
   return 127;
//   EINA_SAFETY_ON_NULL_RETURN_VAL(new_key, 2);
//   EINA_SAFETY_ON_NULL_RETURN_VAL(old_key, 2);
//
//   void *handle;
//   int size;
//   int status = 1;
//
//   handle = eet_read(_ef, old_key, &size);
//   EINA_SAFETY_ON_NULL_GOTO(handle, end);
//
//   size = eet_write(_ef, new_key, handle, size, 1);
//   EINA_SAFETY_ON_FALSE_GOTO(size, end_free);
//
//   size = eet_delete(_ef, old_key);
//   EINA_SAFETY_ON_FALSE_GOTO(size, end_free);
//
//   status = 0;
//
//end_free:
//   free(handle);
//   return status;
}

Eina_Bool
file_entry_exists(const char *key)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(key, EINA_FALSE);

#if COMPRESS
   void *handle;
   handle = eet_read(_ef, key, NULL);
   free(handle);
#else
   const void *handle;
   handle = eet_read_direct(_ef, key, NULL);
#endif

   return (handle != NULL);
}

