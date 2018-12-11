/*
 * Flashcards GUI
 *
 */

#include <iostream>

#include "../gui.h"


using namespace std;



/* show: show a card */
void GUI::show (Card c)
{
    cout << c.question << endl;
}

/* feedback: give feedback to the user; ie Correct or Incorrect */
bool GUI::feedback (string answer, Card question)
{
    if (answer == question.answer)
    {   cout << "Correct" << endl;
        return true;
    }
    else
    {   cout << "Incorrect: Correct answer is '" << question.answer << "'" << endl;
    }
    return false;
}

/* read_answer: read the user's anwser */
string GUI::read_answer()
{
    string str = string();
    getline (cin, str);
    return str;
}



GUI::GUI()
{}

GUI::~GUI()
{}

