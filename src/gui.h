/*
 * Flashcards GUI
 *
 */

#ifndef _GUI_H
#define _GUI_H

#include "user.h"


void gui_write (int x, int y, String str);
void gui_clear(void);

void gui_init(void);
void gui_shutdown(void);


#endif

