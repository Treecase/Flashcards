/* Copyright (C) 2018-2020 Trevor Last
 * See LICENSE file for copyright and license details.
 * cmdline.cpp
 *
 * Flashcards command-line UI
 *
 */

#include "../gui.h"

#include <cstdio>



/* show: show a card */
void GUI::show (struct Card c)
{
    printf("%s\n", c.question.c_str());
}

/* feedback: give feedback to the user; ie Correct or Incorrect */
bool GUI::feedback (std::string answer, struct Card question)
{
    for (std::string ans : question.answers)
    {
        if (answer == ans)
        {
            puts("Correct");
            return true;
        }
    }

    printf("Incorrect: Correct answer: %s",
        question.answers[0].c_str());
    for (
        auto iter = question.answers.cbegin() + 1;
        iter != question.answers.cend();
        ++iter)
    {
        printf(" or %s", (*iter).c_str());
    }
    putchar('\n');
    return false;
}

/* read_answer: read the user's anwser */
std::string GUI::read_answer(void)
{
    char *line = nullptr;
    size_t n = 0;

    ssize_t len = getline(&line, &n, stdin);
    if (len == -1)
    {
        return "";
    }
    std::string out(line, len - 1);
    free(line);
    return out;
}



GUI::GUI()
{
}

GUI::~GUI()
{
}

