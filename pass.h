#ifndef _PASS_H_
#define _PASS_H_

#include <Eina.h>
#include <Eet.h>
#include <Ecore_Getopt.h>
#include <Ecore_File.h>

#include <termios.h>
#include <unistd.h>
#include <sys/mman.h>

extern int _pass_log_dom;

#define CRI(...) EINA_LOG_DOM_CRIT(_pass_log_dom, __VA_ARGS__)
#define ERR(...) EINA_LOG_DOM_ERR(_pass_log_dom, __VA_ARGS__)
#define WRN(...) EINA_LOG_DOM_WARN(_pass_log_dom, __VA_ARGS__)
#define INF(...) EINA_LOG_DOM_INFO(_pass_log_dom, __VA_ARGS__)
#define DBG(...) EINA_LOG_DOM_DBG(_pass_log_dom, __VA_ARGS__)

Eina_Bool file_init(const char *file);
void file_shutdown(void);
Eina_Bool file_entry_exists(const char *key);

int file_stats(void);
int file_list(void);
int file_add(const char *key, const char *data, const char *cipher);
int file_del(const char *key);

/* XXX: This function returns a pointer on a mlock()ed memory page.*/
char *file_get(const char *key, const char *cipher, int *length);
int file_replace(const char *old_key, const char *new_key);

Eina_Bool clipboard_init(void);
void clipboard_shutdown(void);

Eina_Bool tty_init(void);
void tty_shutdown(void);
char *tty_string_silent_get(int *length);

/* XXX: This function returns a pointer on a mlock()ed memory page
 * when safe is set to EINA_TRUE.
 * It is up to the receiver to munlock() it with the received
 * length + 1. Indeed 'length' is the size of the string without
 * the final '\0' */
char *tty_string_get(int *length, Eina_Bool safe);

typedef Eina_Bool (*Clipboard_Set_Func)(const char *data, int data_len);
typedef void (*Output_Func)(const char *format, ...);

extern Clipboard_Set_Func clipboard_set;
extern Output_Func output;

#ifdef HAVE_OSX
Eina_Bool clipboard_cocoa_set(const char *data, int data_len);
#endif
#ifdef HAVE_LINUX
Eina_Bool clipboard_x_set(const char *data, int data_len);
#endif

#define ZERO_MUNLOCK(ptr_, len_) \
   do { \
      memset(ptr_, 0, len_); \
      munlock(ptr_, len_); \
   } while (0)

#define FREE(ptr_) \
   do { \
      free(ptr_); \
      ptr_ = NULL; \
   } while (0)

#endif /* ! _PASS_H_ */

