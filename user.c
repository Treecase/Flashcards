/*
 * Flashcards user interaction
 *
 */

#include <ncurses.h>

#include "user.h"
#include "gui.h"
#include "low.h"

#define gui_clearline(n)    low_clearline (n)



/* show_card: display a flashcard*/
void show_card (Card card) {
    gui_write (0,0, card.question);
}

/* get_answer: read the user's answer */
String get_answer() {

    String r;
    r.str = low_input (-5);
    r.len = wcslen (r.str);

    return r;
}

/* feedback: print feedback (eg correct/incorrect) */
void feedback (char correct, Card card) {
    String s;
    s.str = correct? L"Correct!" : L"Incorrect...";

    gui_clearline (5);
    gui_clearline (7);
    gui_write (0, 5, s);

    if (!correct)
        gui_write (0, 7, card.answer);
}

