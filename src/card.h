/*
 * Card
 *
 */

#ifndef _CARD_H
#define _CARD_H

#include <string>


using namespace std;



/* Card:
 *  A flashcard. Has a Question and an Answer.
 */
class Card
{
public:
    string  question,
            answer;

    bool operator==(Card other)
    {
        if (this->question == other.question
         && this->answer == other.answer)
        {   return true;
        }
        else
        {   return false;
        }
    }

    friend ostream &operator<<(ostream &out, Card &c)
    {   out << (string)"Q: " << c.question << (string)"  A: " << c.answer;
        return out;
    }

    Card()
    {}
    Card (string q, string a)
    {   this->question = q;
        this->answer   = a;
    };
};


#endif

