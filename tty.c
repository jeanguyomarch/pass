#include "pass.h"

static struct termios _old_termios;
static struct termios _new_termios;
static Eina_Bool _hidden = EINA_FALSE;

/*============================================================================*
 *                                   Helpers                                  *
 *============================================================================*/

static inline Eina_Bool
_stdin_hide(void)
{
   int ret;

   if (_hidden == EINA_FALSE)
     {
        ret = tcsetattr(STDIN_FILENO, TCSANOW, &_new_termios);
        if (EINA_UNLIKELY(ret == -1))
          {
             CRI("Failed to mask stdin. Cannot continue because presents "
                 "obvious security risks. Error: %s", strerror(errno));
             return EINA_FALSE;
          }
        _hidden = EINA_TRUE;
     }

   return EINA_TRUE;
}

static inline Eina_Bool
_stdin_restore(void)
{
   int ret;

   if (_hidden == EINA_TRUE)
     {
        ret = tcsetattr(STDIN_FILENO, TCSANOW, &_old_termios);
        if (EINA_UNLIKELY(ret == -1))
          {
             CRI("Failed to restore stdin settings. Something fishy is going "
                 "on there. I stop here. Error: %s", strerror(errno));
             return EINA_FALSE;
          }
        _hidden = EINA_FALSE;
     }

   return EINA_TRUE;
}

/*============================================================================*
 *                                Init/Shutdown                               *
 *============================================================================*/

Eina_Bool
tty_init(void)
{
   int ret;

   /* Save current term, and prepare input mode */
   ret = tcgetattr(STDIN_FILENO, &_old_termios);
   if (EINA_UNLIKELY(ret == -1))
     {
        CRI("Failed to get termios settings for stdin. Error: %s",
            strerror(errno));
        return EINA_FALSE;
     }
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
        if (safe) fpurge(stdin);
        CRI("An error occured while reading stdin");
        goto fail;
     }
   if (safe) fpurge(stdin);

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
   EINA_SAFETY_ON_NULL_RETURN_VAL(length, NULL);

   char *ptr;
   Eina_Bool chk;

   chk = _stdin_hide();
   if (EINA_UNLIKELY(!chk))
     return NULL;

   ptr = tty_string_get(length, EINA_TRUE);
   chk = _stdin_restore();
   if (EINA_UNLIKELY(!chk))
     {
        ZERO_MUNLOCK(ptr, *length);
        FREE(ptr);
        *length = 0;
        return NULL;
     }

   output("\n");
   return ptr;
}

