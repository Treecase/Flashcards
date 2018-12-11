/*
 * Flashcards program
 *
 */

#include "gui.h"
#include "pot.h"

#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <random>

#include <getopt.h>

using namespace std;


void handle_args (int argc, char *argv[]);
unsigned random_int (unsigned max);


unsigned max_pot = 5;
char *deckfile_name = NULL;



/* flashcards: simple flashcard program */
int main (int argc, char *argv[])
{

    handle_args (argc, argv);

    /* read the deckfile */
    ifstream deckfile;
    if (deckfile_name != NULL)
    {
        deckfile = ifstream (deckfile_name);
        if (!deckfile.is_open())
        {   cerr << "failed to open deckfile" << endl;
            return EXIT_FAILURE;
        }
    }
    else
    {   cerr << "NULL deckfile name" << endl;
        return EXIT_FAILURE;
    }

    Deck deck = Deck (deckfile);

    deckfile.close();



    /* create GUI */
    GUI gui = GUI();


    /* create a Pot */
    Pot pot[max_pot];
    pot[0] = Pot (&deck);
    for (unsigned i = 1; i < max_pot; ++i)
    {   pot[i] = Pot();
    }

    for(;;)
    {

        unsigned i;
        Pot p;
        do
        {   i = random_int (max_pot);
            p = pot[i];
        } while (p.size() <= 0);
        //clog << "chose pot #" << i << endl;

        /* pick a random card from the pot */
        Card card = p.pick_card();

        /* display card */
        gui.show (card);

        /* get answer */
        string answer = gui.read_answer();
        if (answer == "")
        {   break;
        }

        /* result */
        bool correct = gui.feedback (answer, card);
        if (correct)
        {
            if (i+1 < max_pot)
            {   pot[i+1].add_card (card);
                p.remove_card (card);
            }
        }
        else
        {   if ((int)(i-1) >= 0)
            {   pot[i-1].add_card (card);
                p.remove_card (card);
            }
        }
    }
    return EXIT_SUCCESS;
}



/* random_int: generate a random int < max*/
unsigned random_int (unsigned max)
{
    int weight_sum = 50;
    int weights[max];

    weights[0] = 50;
    for (unsigned i = 1; i < max; ++i)
    {   weights[i] = weights[i-1] / 2;
        weight_sum += weights[i];
    }

    static random_device rd;
    static mt19937 gen (rd());

    uniform_int_distribution<> dis (0, weight_sum);

    int random_number = dis (gen);
    //clog << "rand = " << random_number << endl;

    for (unsigned i = 0; i < max; ++i)
    {
        if (random_number <= weights[i])
        {   return i;
        }
        random_number -= weights[i];
    }
    cerr << "random_int failed!" << endl;
    return -1;
}

/* handle_args: argv stuff */
void handle_args (int argc, char *argv[])
{
    char const *const shortargs = "";

    int opt;
    while ((opt = getopt (argc, argv, shortargs)) != -1)
    {   switch (opt)
        {
        /* TODO: add some options */
        }
    }
    /* non-option argv elements */
    if (optind < argc)
    {   deckfile_name = argv[optind];
    }
}

