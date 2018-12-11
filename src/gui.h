/*
 * Dummy GUI header; allows easy switching of GUIs
 *
 */



#ifndef __GUI_H
#define __GUI_H


#include <string>

#include "deck.h"


using namespace std;



/* GUI:
 *  Common GUI interface, this allows for easy
 *  substitution of any graphics library.
 */
class GUI
{
private:
    string question,
           answer;
public:
    void   show (Card c);
    bool   feedback (string answer, Card question);
    string read_answer();

    GUI();
    ~GUI();
};


#endif

