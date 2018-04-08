/*
 * Card pots (subdecks)
 *
 */

#include <stdlib.h>
#include <string.h>

#include "pot.h"
#include "error.h"



/* move_card: move a card into a new pot */
void move_card (Card *c, int up) {

    int n = c->pot + up;

    if (n < NUM_POTS && n >= 0) {

        if (add_card (&pot[n], *c))
            error ("add card error!\n");

        pot[n].cards[pot[n].len-1].pot = n;

        int m = -1;
        for (int i = 0; m == -1 && i < pot[c->pot].len; ++i)
            if (!memcmp (&(pot[c->pot].cards[i]), c, sizeof(Card)))
                m = i;
    
        if (m >= 0) {
            if (remove_card (&pot[c->pot], m)) {
                error ("remove card error!\n");
                return;
            }
        }
        else {
            error ("remove_card couldn't find card!\n");
            return;
        }
    }
    else
        error ("out of range %i\n", n);
}

/* remove_card: remove a card from the deck */
int remove_card (Deck *d, int i) {

    if (i < d->len)
        memmove (d->cards+i, d->cards+i+1, sizeof(Card)*(d->len-i));
    memset  (d->cards+d->len, 0, sizeof(Card));

    d->len--;

    return 0;
}

/* add_card: add a card to the deck */
int add_card (Deck *d, Card c) {

    if (!d->cards) {
        d->size  = 25;
        d->len   = 0;
        d->cards = calloc (d->size, sizeof(Card));
        log ("alloc'd deck\n");
    }

    /* resize deck if needed */
    if (d->len >= d->size) {
        if (d->size <= 0) {
            d->size = 5;
            d->len  = 0;
        }
        d->size *= 2;
        d->cards = realloc (d->cards,
            sizeof(Card)*d->size);
        memset (d->cards+d->len, 0,
            sizeof(Card)*(d->size-d->len));
        log ("realloc'd deck\n");
    }
    d->cards[d->len++] = c;

    return 0;
}

