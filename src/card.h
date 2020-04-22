/* Copyright (C) 2018-2020 Trevor Last
 * See LICENSE file for copyright and license details.
 * card.h
 *
 *  Flashcard
 *
 */

#ifndef _CARD_H
#define _CARD_H

#include <string>
#include <vector>


struct Card
{
    std::string question;
    std::vector<std::string> answers;
};


#endif

