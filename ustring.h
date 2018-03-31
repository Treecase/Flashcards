/*
 * String utils
 *
 */

#ifndef _USTRING_H
#define _USTRING_H

#include <wchar.h>

#if WCHAR_MAX < 21
#error "wchar_t is too small!"
#endif


typedef struct {
    wchar_t *str;
    int      len;

    // only used internally
    int      size;
} String;


void str_rappend (String *dest, const char *src);
void str_wrappend (String *dest, const wchar_t *src);
void str_append (String *dest, const String src);

char *str_tombs (const String s);


#endif

