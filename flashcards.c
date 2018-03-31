/*
 * Flashcards program
 *
 * TODO : moving cards b/t pots
 */

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>    // FIXME TEMP
#include <time.h>

#include "gui.h"
#include "user.h"


#define NUM_POTS    5   // TODO user configurable?
static Deck pot[NUM_POTS];



/* read_deck: read the deck file */
Deck read_deck (char *fname) {

    int deck_size = 15;
    Deck d = { calloc (deck_size, sizeof(Card)), 0 };

    FILE *f         = fopen (fname, "r");
    char *input     = NULL;
    long unsigned n = 0;
    char qa, pqa    = -1;
    pqa = qa = -1;

    while ((n = getline (&input, &n, f)) != -1) {

        // next line is question
        if (!strcmp (input, "====\n")) {
            qa = 0;
            d.len++;
        }
        // next line is answer
        else if (!strcmp (input, "---\n"))
            qa = 1;
        else
            input[n-1] = 0;


        if (d.len > deck_size) {
            deck_size *= 2;
            d.cards = realloc (d.cards,
                sizeof(Card)*deck_size);
            memset (d.cards+d.len, 0,
                sizeof(Card)*(deck_size-d.len));
        }


        if (pqa == qa)
            switch (qa) {
            // append to question
            case 0:
                str_rappend (&d.cards[d.len-1].question, input);
                break;
            // append to A
            case 1:
                str_rappend (&d.cards[d.len-1].answer, input);
                break;
            }

        free (input);
        input = NULL;
        n = 0;

        pqa = qa;
    }
    free (input);

    // FIXME
    for (int i = 0; i < d.len; ++i)
        printf ("Q%i: %ls\nA%i: %ls\n====\n",
            i, d.cards[i].question.str,
            i, d.cards[i].answer.str);

    fclose (f);

    return d;
}

/* next_card: get some card from the deck */
Card next_card() {

    Deck d;
    
    do {
        int r = rand() % NUM_POTS;
        d = pot[r];

    } while (d.len == 0 && !d.cards);

    return  d.cards[rand() % d.len];
}

/* check_answer: check if the answer is correct*/
char check_answer (Card c, String a)   {
    return !wcscasecmp (a.str, c.answer.str);
}

/* cleanup: dealloc, etc. */
void cleanup (Deck d) {

    for (int i = 0; i < d.len; ++i) {
        free (d.cards[i].question.str);
        free (d.cards[i].answer.str);
    }
    free (d.cards);
}



int main (int argc, char *argv[]) {

    setlocale (LC_ALL, "");

    srand (time (NULL));

    // TODO proper argv handling
    char *deckfile = (argc > 1)? argv[1] : "deck.dk";

    Deck deck = read_deck (deckfile);

    // TODO encapsulate pot init
    for (int i = 0; i < NUM_POTS; ++i) {
        pot[i].cards = NULL;
        pot[i].len   = 0;
    }

    pot[0] = deck;

    gui_init();

    for (;;) {

        Card card = next_card (deck);

        show_card (card);

        String answer = get_answer();

        clear();

        char correct = check_answer (card, answer);

        feedback (correct, card);
    }

    gui_shutdown();
    cleanup (deck);

    return 0;
}

