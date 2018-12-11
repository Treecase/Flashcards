
Flashcards
==========


This is a simple flashcard program. It makes use of a simple spaced repetition algorithm to improve retention. It also uses a simple-to-edit file format for card decks.

Included are some example decks (Japanese Katakana, Hiragana, Numbers, and basic phrases).


Flashcards are stored together as a deck in a UTF-8 plaintext file. The structure of a card is thus:

```
====
QUESTION
ANSWER
```

Each cards starts with `====`, and ends at either the next `====` or EOF. The QUESTION is the question, obviously, and ANSWER is the answer.

