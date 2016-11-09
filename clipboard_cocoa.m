#import "pass.h"
#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>

static NSPasteboard *_pb = nil;

Eina_Bool
clipboard_cocoa_set(const char *data,
                    int         data_len EINA_UNUSED)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(data, EINA_FALSE);

   NSString *str;

   str = [NSString stringWithCString:data
                           encoding:NSUTF8StringEncoding];

   if (!_pb)
     {
        _pb = [NSPasteboard generalPasteboard];
        [_pb declareTypes:[NSArray arrayWithObject:NSPasteboardTypeString]
                    owner:nil];
     }

   [_pb clearContents];

   return [_pb setData:[str dataUsingEncoding:NSUTF8StringEncoding]
               forType:NSPasteboardTypeString];
}
