/*
 * Flashcards GUI
 *
 */

#include "gui.h"
#include "low.h"



/* gui_write: draw a string to the GUI */
void gui_write (int x, int y, String str) {
    low_drawstring (x, y, str.str);
}

/* gui_init: initialize the GUI */
void gui_init() {
    low_initgraphics();
}

/* gui_shutdown: shutdown the GUI */
void gui_shutdown() {
    low_shutdowngraphics();
}

