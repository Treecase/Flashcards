/*
 * Flashcards GUI in NCurses 
 *
 */

#include <cstring>
#include <ncursesw/ncurses.h>

#include "../gui.h"


#define CENTER_X    (COLS  / 2)
#define CENTER_Y    (LINES / 2)


using namespace std;

#include <fstream>
ofstream log_file;


wchar_t *make_wide (string s);



/* show: show a card */
void GUI::show (Card c)
{
    wchar_t *wstr = make_wide (c.question);
    clear();
    mvaddwstr (CENTER_Y,
              CENTER_X - (c.question.size() / 2),
              wstr);
    refresh();
    delete[] wstr;
}

/* feedback: give feedback to the user; ie Correct or Incorrect */
bool GUI::feedback (string answer, Card question)
{
    clear();
    bool r;
    string str;

    curs_set (0);

    if (answer == question.answer)
    {   str = "Correct";
        r = true;
    }
    else
    {   str = "Incorrect. Correct answer is '" + question.answer + "'";
        r = false;
    }

    wchar_t *wstr = make_wide (str);
    mvaddwstr (CENTER_Y, CENTER_X - (str.size() / 2), wstr);
    delete[] wstr;

    refresh();
    getch();

    return r;
}

/* read_answer: read the user's answer */
string GUI::read_answer()
{
    curs_set (2);

    string str = string();

    move (CENTER_Y+2, CENTER_X);

    bool done = false;
    while (!done)
    {
        int c = getch();

        /* context-dependent ^D interpretation:
         *  When it is the only character -- exit
         *  Otherwise, treat it as an enter */
        if ((char)c == '')
        {   if (str.size() == 0)
            {   c = KEY_EXIT;
            }
            else
            {   c = KEY_ENTER;
            }
        }

        switch (c)
        {

        case KEY_BACKSPACE:
            if (str.size() > 0)
            {   str.erase (str.begin() + str.size()-1);
                int y = getcury (stdscr),
                    x = getcurx (stdscr);
                mvaddch (y, x-1, ' ');
                move (y, x-1);
            }
            break;

        case ERR:
            cerr << "getch failed - " << strerror (errno);
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
            addch (c);
            break;
        }

        /* write the string to the screen, centered */
        wchar_t *wstr = make_wide (str);
        move (CENTER_Y+2, 0);
        for (int i = 0; i < COLS; ++i)
        {   addch (' ');
        }
        mvaddwstr (CENTER_Y+2, CENTER_X - (str.size() / 2), wstr);
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
    keypad (stdscr, TRUE);

    log_file.open ("log.txt");
}

GUI::~GUI()
{   endwin();
}



/* UTILITY FUNCTIONS */
/* make_wide: convert a UTF-8 string to wide chars */
wchar_t *make_wide (string s)
{
    char const *mbs = s.c_str();
    size_t len = mbstowcs (NULL, mbs, 0)+1;

    wchar_t *wide = new wchar_t[len];
    mbstowcs (wide, mbs, len);

    return wide;
}

