/* Copyright (C) 2018-2020 Trevor Last
 * See LICENSE file for copyright and license details.
 * gui.h
 *
 * Flashcards frontend interface
 *
 */



#ifndef _GUI_H
#define _GUI_H

#include "card.h"

#include <string>


/* GUI:
 *  Common GUI interface, this allows for easy
 *  substitution of any graphics library.
 */
class GUI
{
private:
    std::string question,
                answer;

public:
    void show(struct Card c);
    bool feedback(std::string answer, struct Card question);
    std::string read_answer();

    GUI();
    ~GUI();
};


#endif

