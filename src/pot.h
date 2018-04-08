/*
 * Card pots (subdecks)
 *
 */

#ifndef _POT_H
#define _POT_H

#include "user.h"


unsigned int NUM_POTS;
Deck *pot;



void move_card (Card *c, int up);

int remove_card (Deck *d, int i);

int add_card (Deck *d, Card c);


#endif

