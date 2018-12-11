/*
 * Deck
 *
 */

#ifndef _DECK_H
#define _DECK_H

#include "card.h"

#include <fstream>
#include <vector>
#include <iostream> /* temp */


using namespace std;



/* Deck:
 *  A list of Cards.
 */
class Deck
{
private:
    vector<Card> cards;
public:
    unsigned long size()
    {   return (int)this->cards.size();   }

    Card operator[](unsigned long i)
    {
        Card c = this->cards[i];
        return c;
    }

    friend ostream& operator<<(ostream &out, Deck &d)
    {   out << (string)"vector<";
        for (unsigned long i = 0; i < d.size(); ++i)
        {   out << d.cards[i] << ", ";
        }
        out << (string)">";
        return out;
    }

    void remove (unsigned long i)
    {   this->cards.erase (this->cards.begin()+i);   }

    void add (Card c)
    {   this->cards.push_back (Card(c.question, c.answer));
    }

    Deck();
    Deck (ifstream &f);
};


#endif

