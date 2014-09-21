#ifndef _PASS_H_
#define _PASS_H_

#include <Eina.h>
#include <Eet.h>
#include <Ecore_Getopt.h>
#include <Ecore_File.h>

#include <termios.h>
#include <unistd.h>

extern int _pass_log_dom;

#define CRI(...) EINA_LOG_DOM_CRIT(_pass_log_dom, __VA_ARGS__)
#define ERR(...) EINA_LOG_DOM_ERR(_pass_log_dom, __VA_ARGS__)
#define WRN(...) EINA_LOG_DOM_WARN(_pass_log_dom, __VA_ARGS__)
#define INF(...) EINA_LOG_DOM_INFO(_pass_log_dom, __VA_ARGS__)
#define DBG(...) EINA_LOG_DOM_DBG(_pass_log_dom, __VA_ARGS__)

Eina_Bool file_init(void);
void file_shutdown(void);
Eina_Bool file_entry_exists(const char *key);

int file_stats(void);
int file_list(void);
int file_add(const char *key, const char *data, const char *cipher);
int file_del(const char *key);
char *file_get(const char *key, const char *cipher);

Eina_Bool clipboard_init(void);
void clipboard_shutdown(void);

Eina_Bool tty_init(void);
void tty_shutdown(void);
char *tty_string_silent_get(int *length);
char *tty_string_get(int *length);

typedef Eina_Bool (*Clipboard_Set_Func)(const char *data, int data_len);
typedef void (*Output_Func)(const char *format, ...);

extern Clipboard_Set_Func clipboard_set;
extern Output_Func output;

#ifdef HAVE_OSX
Eina_Bool clipboard_cocoa_set(const char *data, int data_len);
#endif

#endif /* ! _PASS_H_ */

