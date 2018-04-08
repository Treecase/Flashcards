/*
 * Flashcards program
 *
 */

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "gui.h"
#include "user.h"
#include "pot.h"
#include "error.h"


int  DEBUG = 0;
char GLOBAL_exit = 0;
char GLOBAL_loglevel = LOG_SILENT;

static Deck deck;


unsigned long *TIMES_PICKED;
unsigned long *TOTAL_TIMES_PICKED;
unsigned long NUM_CARDS                     = 0;
unsigned long TOTAL_NUM_RANDS               = 0;



/* read_deck: read the deck file */
Deck read_deck (char *fname) {

    Deck d = { calloc (15, sizeof(Card)), 0, 15 };

    errno = 0;
    FILE *f         = fopen (fname, "r");
    char *input     = NULL;
    long unsigned n = 0;
    char qa, pqa    = -1;
    pqa = qa = -1;
    
    if (!f) {
        fatal ("%s: %s\n", fname, strerror (errno));
    }

    while ((n = getline (&input, &n, f)) != -1) {

        /* ==== means next line is question */
        if (!strcmp (input, "====\n")) {
            qa = 0;
            d.len++;
        }
        /* --- means next line is answer */
        else if (!strcmp (input, "---\n"))
            qa = 1;
        else
            input[n-1] = 0;


        /* resize deck if needed */
        if (d.len >= d.size) {

            int oldsiz = d.size;
            d.size *= 2;
            d.cards = realloc (d.cards,
                sizeof(Card)*d.size);

            memset (d.cards+oldsiz, 0,
                sizeof(Card)*(d.size-oldsiz));
        }


        if (pqa == qa)
            switch (qa) {
            /* append to question */
            case 0:
                str_rappend (&d.cards[d.len-1].question, input);
                break;
            /* append to answer */
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
    fclose (f);

    return d;
}

/* next_card: get some card from the deck
  > each pot has a probability of 1/2 the
    probability of the previous. ie. pot0
    has 50% chance, pot1 has 25% chance,
    etc. of being picked. */
Card *next_card(void) {


    int probs[NUM_POTS];
    int sum = 0;
    for (int i = 1, prob = 100; i <= NUM_POTS; ++i) {
        sum += (prob /= 2);
        probs[i-1] = prob;
        log ("prob %i is %i%%\n", i-1, prob);
    }

    Deck d = { NULL, 0, 0 };
    int i;

    do {
        unsigned int r = rand() % sum;
        for (i = 0; i < NUM_POTS; ++i) {
            if (r < probs[i]) {
                d = pot[i];
                break;
            }
            r -= probs[i];
        }
        log ("picking pot %i\n", i);

        TOTAL_NUM_RANDS++;
        TOTAL_TIMES_PICKED[i]++;
    } while (d.len == 0 || !d.cards);

    log ("picked pot %i\n\n", i);

    TIMES_PICKED[i]++;
    log ("pot %i has been picked %lu times (%f%%)\n", i,
        TIMES_PICKED[i],
        100.0*(double)TIMES_PICKED[i]/++NUM_CARDS);

    return  &d.cards[rand() % d.len];
}

/* check_answer: check if the answer is correct*/
char check_answer (Card c, String a)   {
    return !wcscasecmp (a.str, c.answer.str);
}

/* cleanup: dealloc, etc. */
void cleanup(void) {

    for (int i = 0; i < deck.len; ++i) {
        free (deck.cards[i].question.str);
        deck.cards[i].question.str = NULL;

        free (deck.cards[i].answer.str);
        deck.cards[i].answer.str   = NULL;
    }

    for (int i = 0; pot && i < NUM_POTS; ++i) {
        for (int j = 0; j < pot[i].len; ++j) {

            if (pot[i].cards[j].question.str) {
                free (pot[i].cards[j].question.str);
                pot[i].cards[j].question.str = NULL;
            }

            if (pot[i].cards[j].answer.str) {
                free (pot[i].cards[j].answer.str);
                pot[i].cards[j].answer.str = NULL;
            }
        }
        free (pot[i].cards);
        pot[i].cards = NULL;
        free (pot);
        pot = NULL;
    }
    free (TIMES_PICKED);
    free (TOTAL_TIMES_PICKED);
}



/* flashcards: simple flashcard program */
int main (int argc, char *argv[]) {

    /* startup stuff */
    DEBUG = fileno (stderr);
    setlocale (LC_ALL, "");
    srand (time (NULL));

    /* pots stuff */
    NUM_POTS = 5;

    TIMES_PICKED = calloc (NUM_POTS, sizeof(*TIMES_PICKED));
    TOTAL_TIMES_PICKED = calloc (NUM_POTS, sizeof(*TOTAL_TIMES_PICKED));

    /* register cleanup functions */
    atexit (gui_shutdown);
    atexit (cleanup);


    /* argv handling */
    char *deckfile = NULL;
    char skip_next_arg = 0;
    for (int i = 1; i < argc; ++i) {
        skip_next_arg = 0;

        if (argv[i][0] == '-') {
            for (int c = 1; c < strlen (argv[i]); ++c)
                switch (argv[i][c]) {
                /* -s : silent, no logging */
                case 's':
                    GLOBAL_loglevel = LOG_SILENT;
                    break;

                /* -d : debug mode */
                case 'd':
                    GLOBAL_loglevel = LOG_DEBUG;
                    break;

                /* -n : number of pots */
                case 'n':
                    if (i+1 < argc) {
                        NUM_POTS = atoi (argv[i+1]);
                        skip_next_arg = 1;
                    }
                    else
                        fatal ("-n option requires an argument!\n");
                    break;

                /* unknown option */
                default:
                    fatal ("unknown option '-%c'\n", argv[i][c]);
                    break;
                }
            
        }
        /* interpret as deckfile name */
        else
            deckfile = strdup (argv[i]);

        if (skip_next_arg)
            ++i;
    }
    if (!deckfile)
        fatal ("no deckfile supplied!\n");


    deck = read_deck (deckfile);
    if (!deck.len)
        fatal ("deckfile '%s' contains no valid cards!\n", deckfile);

    /* initialize pots */
    pot = calloc (NUM_POTS, sizeof(*pot));
    pot[0] = deck;


    /*  startup gui, switch to .txt
        logging, because ncurses */
    gui_init();

    /* main loop */
    for (unsigned long num_loops = 0;; ++num_loops) {

        Card *card = next_card();
        log ("card is '%ls' (%ls)\n", card->question.str, card->answer.str);

        show_card (*card);

        String answer = get_answer();

        if (GLOBAL_exit) {
            free (answer.str);
            break;
        }

        gui_clear();

        char correct = check_answer (*card, answer);
        free (answer.str);

        feedback (correct, *card);

        log ("moving '%ls' (%ls) to pot %i\n", card->question.str,
            card->answer.str, card->pot + (correct? 1 : -1));
        move_card (card, correct? 1 : -1);

        /* debugging */
        for (int p = 0; p < NUM_POTS; ++p) {
            log ("<== (%p) POT %i (%i) ==>\n", pot+p, p, pot[p].len);
            for (int i = 0; i < pot[p].len; ++i)
                log ("'%ls' (%ls) > %i\n", pot[p].cards[i].question.str,
                    pot[p].cards[i].answer.str, pot[p].cards[i].pot);
            log ("<== /POT ==>\n");
        }
        log ("LOOP #%lu\n", num_loops);
    }

    /* shutdown GUI, cleanup */
    free (deckfile);

    /* debugging */
    for (int i = 0; i < NUM_POTS; ++i)
        log ("pot %i has been picked %lu times (%f%%),"
             "and %lu times total (%f%%)\n", i, TIMES_PICKED[i],
            100.0*(double)TIMES_PICKED[i]/NUM_CARDS,
            TOTAL_TIMES_PICKED[i],
            100.0*(double)TOTAL_TIMES_PICKED[i]/TOTAL_NUM_RANDS);

    log ("CLEANUP COMPLETE");

    return 0;
}

