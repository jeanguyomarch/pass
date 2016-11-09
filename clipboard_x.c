#include "pass.h"

static Eina_Bool
_escape_dquotes(const char *src,
                char       *dst,
                int         dst_len)
{
   char *ptr = (char *)src;
   char c;
   int k = 0;

   while ((c = *(ptr++)))
     {
        if (c == '"')
          {
             if (k >= dst_len) return EINA_FALSE;
             dst[k++] = '\\';
          }
        if (k >= dst_len) return EINA_FALSE;
        dst[k++] = c;
     }

   if (k >= dst_len) return EINA_FALSE;
   dst[k] = 0;

   return EINA_TRUE;
}

Eina_Bool
clipboard_x_set(const char *data,
                int         data_len EINA_UNUSED)
{
   char esc[1024];
   char buf[2048];
   Eina_Bool ok;
   int ret;

   ok = _escape_dquotes(data, esc, sizeof(esc));
   if (EINA_UNLIKELY(!ok))
     {
        CRI("Failed to escape password");
        return EINA_FALSE;
     }

   snprintf(buf, sizeof(buf), "echo \"%s\" | xclip -selection clipboard", esc);
   ret = system(buf);
   if (ret != 0)
     {
        ERR("Failed to set clipboard via xclip --- is xclip in your PATH or installed?");
        return EINA_FALSE;
     }

   return EINA_TRUE;
}
