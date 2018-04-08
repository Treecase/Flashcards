/*
 * Flashcards GUI
 *
 */

#include <ncurses.h>

#include "gui.h"
#include "low.h"



/* write: draw a string to the GUI */
inline void gui_write (int x, int y, String str) {
    low_drawstring (x, y, str.str);
}

/* clear: clear the screen */
inline void gui_clear(void) {
    clear();
}

/* init: initialize the GUI */
inline void gui_init(void) {
    low_initgraphics();
}

/* shutdown: shutdown the GUI */
inline void gui_shutdown(void) {
    low_shutdowngraphics();
}

