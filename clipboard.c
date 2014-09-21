#include "pass.h"

/*============================================================================*
 *                                OSX Clipboard                               *
 *============================================================================*/

#ifdef HAVE_OSX
static Eina_Bool
_osx_clipboard_set(const char *data,
                   int         data_len)
{
   return EINA_TRUE;
}
#endif


/*============================================================================*
 *                                Init/Shutdown                               *
 *============================================================================*/

Eina_Bool
clipboard_init(void)
{
#ifdef HAVE_OSX
   clipboard_set = _osx_clipboard_set;
#endif

   if (EINA_UNLIKELY(clipboard_set == NULL))
     {
        CRI("Sorry, your clipboard mechanism is not implemented. Please contribute!");
        return EINA_FALSE;
     }

   return EINA_TRUE;
}

void
clipboard_shutdown(void)
{
   clipboard_set = NULL;
}

