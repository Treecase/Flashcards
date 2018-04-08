/*
 * Flashcards user interaction
 *
 */

#ifndef _USER_H
#define _USER_H

#include "ustring.h"



/* TODO MOVE Card DEF */
typedef struct {
    String  question,
            answer;
    int     pot;
} Card;

/* TODO MOVE Deck DEF */
typedef struct {
    Card *cards;
    int   len,
          size;
} Deck;


void show_card (Card card);

String get_answer(void);

void feedback (char correct, Card card);


#endif

