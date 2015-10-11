#include "pass.h"
#include <Elementary.h>

Eina_Bool
clipboard_elm_set(const char *data,
                  int         data_len)
{
   return elm_cnp_selection_set(NULL, ELM_SEL_TYPE_CLIPBOARD,
                                ELM_SEL_FORMAT_TEXT, data, data_len);
}

