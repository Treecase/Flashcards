/*
 * Card pots
 *
 */

#ifndef _POT_H
#define _POT_H

#include "deck.h"
#include "card.h"


class Pot
{
protected:
    Deck *deck;
public:
    Card pick_card();
    void remove_card (Card c);
    void add_card (Card c);
    int size()
    {   return this->deck->size();   };

    /* temp */
    Card operator[](unsigned long i)
    {
        Card c = (*this->deck)[i];
        return c;
    }

    friend ostream &operator<<(ostream &out, Pot &p)
    {
        out << (string)"Pot(" << *p.deck << (string)")";
        return out;
    }

    Pot();
    Pot (Deck *d);
};


#endif

