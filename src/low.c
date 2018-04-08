/*
 * Flashcards low-level utils
 *
 */

#define _XOPEN_SOURCE_EXTENDED  /* wide character ncurses */
#include <ncursesw/ncurses.h>
#include <locale.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "low.h"
#include "error.h"



/* drawstring: draw a string to the screen */
void low_drawstring (int x, int y, wchar_t *str) {
    if (str != NULL)
        mvaddwstr (CENTER_Y+y, (CENTER_X+x)-(wcslen (str)/2), str);
}

/* clearline: make a line blank */
void low_clearline (int l) {
    int x,y;
    getyx (stdscr, y, x);
    move (CENTER_Y+l, 0);
    clrtoeol();
    move (y, x);
}

/* input: get user input */
wchar_t *low_input (int y) {

    int cy, cx;
    char skipc;
    cx = 0;

    int i       = 0;
    int strsiz  = 24;
    int strlen  = 0;
    wchar_t *str = calloc (strsiz, sizeof(wint_t));
    wint_t c;

    low_clearline (CENTER_Y+y);
    move (CENTER_Y+y, CENTER_X);
    attron (A_BOLD);

    get_wch (&c);
    while (c != '\n' && c != '\r') {

        skipc = 1;

        switch (c) {
        case KEY_BACKSPACE:
            if (i > 0) {
                --i;
                wmemmove (str+i, str+i+1, strlen-i);
                --strlen;
            }
            break;
        case KEY_LEFT:
            if (i > 0) {
                --i;
                --cx;
            }
            break;
        case KEY_RIGHT:
            if (i < strlen) {
                ++i;
                ++cx;
            }
            break;

        /* FIXME remove magic numbers */
        /* ESC key */
        case 27:
        /* ^C */
        case 3:
        /* ^D */
        case 4:
            GLOBAL_exit = 1;
            free (str);
            return NULL;
            break;

        default:
            skipc = 0;
            break;
        }

        if (!skipc) {
            str[i++] = c;
            if (i > strlen)
                ++strlen;
            wmemset (str+strlen, 0, strsiz-strlen);
        }

        cy = getcury (stdscr);
        low_clearline (cy-CENTER_Y);
        mvaddwstr (cy, CENTER_X-(i/2), (wchar_t *)str);
        move (cy, getcurx (stdscr)+cx);
        refresh();

        if (i >= strsiz) {
            strsiz *= 2;
            str = realloc (str, sizeof(wint_t)*strsiz);
        }
        get_wch (&c);
    }
    attroff (A_BOLD);

    return str;
}

/* initgraphics: initialize graphics stuff */
void low_initgraphics(void) {

    initscr();
    raw();
    noecho();

    nonl();
    intrflush (stdscr, FALSE);
    keypad (stdscr, TRUE);

    atexit (low_shutdowngraphics);

    /* DEBUG is global; the debug output file */
    DEBUG = creat ("debug.txt", S_IRUSR | S_IWUSR);
}

/* shutdowngraphics: shutdown graphics stuff */
void low_shutdowngraphics(void) {
    endwin();

    close (DEBUG);
}

