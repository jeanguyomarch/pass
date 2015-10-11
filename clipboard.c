#include "pass.h"

/*============================================================================*
 *                                Init/Shutdown                               *
 *============================================================================*/

Eina_Bool
clipboard_init(void)
{
#ifdef HAVE_OSX
   clipboard_set = clipboard_cocoa_set;
#endif
#ifdef HAVE_LINUX
   clipboard_set = clipboard_elm_set;
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

