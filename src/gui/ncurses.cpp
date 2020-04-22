/* Copyright (C) 2018-2020 Trevor Last
 * See LICENSE file for copyright and license details.
 * ncurses.cpp
 *
 * Flashcards NCurses UI
 *
 */

#include "../gui.h"

#include <cstring>

#include <ncursesw/ncurses.h>


#define CENTER_X    (COLS  / 2)
#define CENTER_Y    (LINES / 2)



FILE *log_file = nullptr;


wchar_t *make_wide(std::string s);



/* show: show a card */
void GUI::show(Card c)
{
    wchar_t *wstr = make_wide(c.question);
    clear();
    mvaddwstr(CENTER_Y, CENTER_X - (c.question.size() / 2), wstr);
    refresh();
    delete[] wstr;
}

/* feedback: give feedback to the user; ie Correct or Incorrect */
bool GUI::feedback(std::string answer, Card question)
{
    clear();
    bool result = false;
    std::string str = "";

    curs_set(0);

    for (std::string ans : question.answers)
    {
        if (answer == ans)
        {
            str = "Correct";
            result = true;
            break;
        }
    }
    if (!result)
    {
        str = "Incorrect. Correct answer: " + question.answers[0];
        for (
            auto iter = question.answers.cbegin()+1;
            iter != question.answers.cend();
            ++iter)
        {
            str += " or " + *iter;
        }
    }

    wchar_t *wstr = make_wide(str);
    mvaddwstr(CENTER_Y, CENTER_X - (str.size() / 2), wstr);
    delete[] wstr;

    refresh();
    getch();

    return result;
}

/* read_answer: read the user's answer */
std::string GUI::read_answer()
{
    curs_set(2);

    std::string str = "";

    move(CENTER_Y+2, CENTER_X);

    bool done = false;
    while (!done)
    {
        int c = getch();

        /* context-dependent ^D interpretation:
         *  When it is the only character -- exit
         *  Otherwise, treat it as an enter */
        if ((char)c == '')
        {
            if (str.size() == 0)
            {
                c = KEY_EXIT;
            }
            else
            {
                c = KEY_ENTER;
            }
        }

        switch (c)
        {
        case KEY_BACKSPACE:
            if (str.size() > 0)
            {
                str.erase(str.begin() + str.size()-1);
                int y = getcury(stdscr),
                    x = getcurx(stdscr);
                mvaddch(y, x-1, ' ');
                move(y, x-1);
            }
            break;

        case ERR:
            fprintf(stderr, "getch failed - %s\n", strerror(errno));
            /* fallthrough */
        case KEY_ENTER:
        case '\n':
        case '\r':
        case KEY_BREAK:
        case KEY_EXIT:
        case KEY_CLOSE:
        case KEY_CANCEL:
        case '':
            done = true;
            break;

        default:
            str += c;
            addch(c);
            break;
        }

        /* write the string to the screen, centered */
        wchar_t *wstr = make_wide(str);
        move(CENTER_Y+2, 0);
        for (int i = 0; i < COLS; ++i)
        {
            addch(' ');
        }
        mvaddwstr(CENTER_Y+2, CENTER_X - (str.size() / 2), wstr);
        refresh();
        delete[] wstr;
    }
    return str;
}



GUI::GUI()
{
    setlocale(LC_ALL, "");

    initscr();
    /* unbuffered input */
    cbreak();
    /* don't echo as user types */
    noecho();
    /* enable the keypad */
    keypad(stdscr, TRUE);

    log_file = fopen("log.txt", "r");
}

GUI::~GUI()
{
    endwin();
}



/* UTILITY FUNCTIONS */
/* make_wide: convert a UTF-8 string to wide chars */
wchar_t *make_wide(std::string s)
{
    char const *mbs = s.c_str();
    size_t len = mbstowcs(NULL, mbs, 0)+1;

    wchar_t *wide = new wchar_t[len];
    mbstowcs(wide, mbs, len);

    return wide;
}

