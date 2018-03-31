/*
 * String utils
 *
 */

#include <stdio.h>  // FIXME TEMP

#include <stdlib.h>
#include <string.h>

#include "ustring.h"



/* rappend: append regular chars to a string */
void str_rappend (String *dest, const char *src) {

    // convert mbs to wchar string
    int srclen = strlen (src);
    unsigned long wsrclen   = mbstowcs (NULL, src, 0);
    wchar_t *wsrc = malloc (sizeof(wchar_t) * wsrclen);
    mbstowcs (wsrc, src, wsrclen);


    // alloc if str is NULL
    if (!dest->str) {
        dest->str  = calloc (srclen+1, sizeof(wchar_t));
        dest->size = wsrclen+1;
        dest->len  = 0;
    }

    // resize if needed
    while (dest->size < wsrclen+dest->len) {
        if (dest->size <= 0)
            dest->size = 5;

        dest->size *= 2;
        wchar_t *temp = realloc (dest->str,
            sizeof(wchar_t)*dest->size);

        if (!temp)
            return; // FIXME error

        dest->str = temp;
        memset (dest->str+dest->len, 0,
            sizeof(wchar_t)*(dest->size - dest->len));
    }
    // append
    for (int i = 0; i < wsrclen; ++i)
        dest->str[dest->len+i] = wsrc[i];
    dest->len += wsrclen;

    free (wsrc);
}

/* wrappend: append wide chars to a string */
void str_wrappend (String *dest, const wchar_t *src) {

    int n = wcstombs (NULL, src, 0)+1;
    char *temp = malloc (n);
    wcstombs (temp, src, n);

    str_rappend (dest, temp);
}

/* append: append two strings */
void str_append (String *dest, const String src) {
    str_wrappend (dest, src.str);
}

/* tombs: convert wcs string to a mbs */
char *str_tombs (const String str) {

    unsigned long len = wcstombs (NULL, str.str, 0);
    char *o = malloc (len+1);
    wcstombs (o, str.str, len);

    return o;
}

