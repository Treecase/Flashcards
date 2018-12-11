/*
 * Card pots
 *
 */

#include <cstdlib>

#include "pot.h"



/* pick_card: return a random card in the pot */
Card Pot::pick_card()
{
    unsigned long rand_num = rand() % this->size();
    Card c = ((*this->deck))[rand_num];
    return c;
}

/* remove_card: remove a card from the pot */
void Pot::remove_card (Card c)
{
    bool found = false;
    for (int i = 0; i < this->size(); ++i)
    {   if ((*this->deck)[i] == c)
        {   this->deck->remove (i);
            found = true;
            break;
        }
    }
    if (!found)
    {   cerr << "failed to find card" << endl;
    }
}

/* add_card: add a card to the pot */
void Pot::add_card (Card c)
{   this->deck->add (c);
}


Pot::Pot()
{   this->deck = new Deck();
}

Pot::Pot (Deck *d)
{   this->deck = d;
}

