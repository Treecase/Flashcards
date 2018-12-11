/*
 * Flashcards user interaction
 *
 */

#include <vector>

#include "deck.h"



Deck::Deck()
{
    this->cards = vector<Card>();
}

Deck::Deck (ifstream &f)
{
    this->cards = vector<Card>();
    string line = string();

    while (getline (f, line))
    {
        if (line == "====")
        {
            string q   = string(),
                   a   = string(),
                   tmp = string();

            /* read the question */
            while (getline (f, tmp) && tmp != "---")
            {   q += tmp;
            }

            /* read the answer to the question */
            while (getline (f, tmp) && tmp != "====")
            {   a += tmp;
            }

            this->cards.push_back (Card(q, a));
        }
    }
}

