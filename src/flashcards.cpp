/* Copyright (C) 2018-2020 Trevor Last
 * See LICENSE file for copyright and license details.
 * flashcards.cpp
 *
 * Flashcards program
 *
 */

#include "card.h"
#include "gui.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include <getopt.h>

#include <string>
#include <vector>



std::vector<Card> parse_deck(FILE *f);



/* flashcards: simple flashcard program */
int main(int argc, char *argv[])
{
    srand(time(NULL));

    char *deckfile_name = nullptr;
    if (argc > 1)
    {
        deckfile_name = argv[1];
    }
    else
    {
        fprintf(stderr, "No deckfile given!\n");
        exit(EXIT_FAILURE);
    }

    errno = 0;
    FILE *deckfile = fopen(deckfile_name, "r");
    if (deckfile == nullptr)
    {
        fprintf(
            stderr,
            "fopen(\"%s\") -- %s\n",
            deckfile_name,
            strerror(errno));
        exit(EXIT_FAILURE);
    }

    std::vector<std::vector<Card>> pots{ parse_deck(deckfile) };
    fclose(deckfile);

    GUI gui{};


    for(;;)
    {
        std::vector<double> weights{};
        for (auto pot : pots)
        {
            if (weights.size() == 0)
            {
                weights.push_back(1.0);
            }
            else
            {
                weights.push_back(weights.back() / 2.0);
            }
        }

        size_t pot_idx = 0;
        do
        {
            double val = (rand() % 1000) / 1000.0;
            for (ssize_t i = weights.size()-1; i >= 0; --i)
            {
                if (val < weights[i])
                {
                    pot_idx = i;
                    break;
                }
            }
        } while (pots[pot_idx].empty());
        auto &pot = pots[pot_idx];

        size_t idx = rand() % pot.size();
        Card card = pot[idx];

        gui.show(card);
        std::string ans = gui.read_answer();
        if (ans == "")
        {
            break;
        }

        pot.erase(pot.cbegin() + idx);
        if (gui.feedback(ans, card))
        {
            if (pot_idx + 1 >= pots.size())
            {
                pots.push_back(std::vector<Card>{});
            }
            pots[pot_idx + 1].push_back(card);

            if (pots.front().empty())
            {
                pots.erase(pots.cbegin());
            }
        }
        else
        {
            if (pot_idx == 0)
            {
                pots.insert(pots.cbegin(), std::vector<Card>{});
            }
            pots[pot_idx].push_back(card);

            if (pots.back().empty())
            {
                pots.erase(pots.cend());
            }
        }
    }

    return EXIT_SUCCESS;
}


/* parse a deckfile */
std::vector<Card> parse_deck(FILE *f)
{
    std::vector<Card> deck{};

    char *text = (char *)calloc(10, sizeof(*text));
    size_t n = 10;

    Card *card = nullptr;

    for(;;)
    {
        errno = 0;
        ssize_t len = getdelim(&text, &n, ';', f);
        if (len == -1)
        {
            if (errno != 0)
            {
                fprintf(stderr, "getdelim -- %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
            else
            {
                /* hit EOF */
                if (card == nullptr)
                {
                    fprintf(stderr, "Deckfile contains no cards!\n");
                    exit(EXIT_FAILURE);
                }
                deck.push_back(*card);
                delete card;
                break;
            }
        }

        bool done = false;
        for (ssize_t i = 0; !done && i < len; ++i)
        {
            if (!isspace(text[i]))
            {
                if (i+1 < len && text[i+1] == ':')
                {
                    switch (text[i])
                    {
                    case 'Q':
                        if (card != nullptr)
                        {
                            deck.push_back(*card);
                            delete card;
                        }
                        card = new Card{};
                        card->question = std::string(
                            text + i + 2,
                            len - i - 3);
                        done = true;
                        break;
                    case 'A':
                        card->answers.push_back(
                            std::string(text + i + 2, len - i - 3));
                        done = true;
                        break;

                    default:
                        fprintf(
                            stderr,
                            "error -- unexpected character '%c'\n",
                            text[i]);
                        exit(EXIT_FAILURE);
                        break;
                    }
                }
                else
                {
                    fprintf(
                        stderr,
                        "error -- unexpected character '%c'\n",
                        text[i]);
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
    free(text);

    return deck;
}

