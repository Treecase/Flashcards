/*
 * Flashcards low-level utils
 *
 */

#ifndef _LOW_H
#define _LOW_H

#include <wchar.h>

#define CENTER_X    (COLS/2)
#define CENTER_Y    (LINES/2)


void low_drawstring (int x, int y, wchar_t *str);

void low_clearline (int l);

wchar_t *low_input (int y);

void low_initgraphics();
void low_shutdowngraphics();


#endif

