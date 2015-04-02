#include "pass.h"

static struct termios _old_termios;
static struct termios _new_termios;
static Eina_Bool _hidden = EINA_FALSE;

/*============================================================================*
 *                                   Helpers                                  *
 *============================================================================*/

static inline void
_stdin_hide(void)
{
   if (_hidden == EINA_FALSE)
     {
        tcsetattr(STDIN_FILENO, TCSANOW, &_new_termios);
        _hidden = EINA_TRUE;
     }

}

static inline void
_stdin_restore(void)
{
   if (_hidden == EINA_TRUE)
     {
        tcsetattr(STDIN_FILENO, TCSANOW, &_old_termios);
        _hidden = EINA_FALSE;
     }
}

/*============================================================================*
 *                                Init/Shutdown                               *
 *============================================================================*/

Eina_Bool
tty_init(void)
{
   /* Save current term, and prepare input mode */
   tcgetattr(STDIN_FILENO, &_old_termios);
   memcpy(&_new_termios, &_old_termios, sizeof(struct termios));

   /* Silent input */
   _new_termios.c_lflag &= (~ECHO);

   return EINA_TRUE;
}

void
tty_shutdown(void)
{
   /* Always restore old behaviour */
   _stdin_restore();
}

/*============================================================================*
 *                                 Get Strings                                *
 *============================================================================*/

char *
tty_string_get(int       *length,
               Eina_Bool  safe)
{
   char *ptr, *tmp;
   char buf[512] = {};
   int len;

   /* Never swap this */
   if (safe) mlock(buf, sizeof(buf));

   /* Get string from stdin (safe) */
   ptr = fgets(buf, sizeof(buf), stdin);

   if (feof(stdin) || ferror(stdin) || ptr == NULL)
     {
        CRI("An error occured while reading stdin");
        goto fail;
     }

   /* Control the size */
   len = strlen(ptr);
   if (len >= sizeof(buf) - 1)
     {
        ERR("Your text is %i bytes. The maximum allowed is %zu.",
            len, sizeof(buf) - 1);
        goto fail;
     }

   /* Removing trailing newline */
   ptr[len - 1] = 0;

   /* Allocate return buffer */
   tmp = malloc(len);
   if (EINA_UNLIKELY(tmp == NULL))
     {
        CRI("Failed to allocate memory");
        goto fail;
     }

   /* Never swap this */
   if (safe) mlock(tmp, len); /* Receiver is resposible of munlock()ing */
   memcpy(tmp, buf, len);

   memset(buf, 0, sizeof(buf));
   if (safe) munlock(buf, sizeof(buf));

   if (length) *length = len - 1;
   len = 0;
   return tmp;

fail:
   memset(buf, 0, sizeof(buf));
   if (safe) munlock(buf, sizeof(buf));
   if (length) *length = -1;
   len = 0;
   return NULL;
}

char *
tty_string_silent_get(int *length)
{
   char *ptr;

   _stdin_hide();
   ptr = tty_string_get(length, EINA_TRUE);
   _stdin_restore();
   output("\n");

   return ptr;
}

