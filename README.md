
Flashcards
==========


This is a simple flashcard program. It makes use of a simple spaced repetition algroithm to improve retention. It also uses a simple-to-edit file format for card decks.

Included are some example decks (Japanese katakana and hiragana).


Flashcards are stored together as a deck in a plaintext file. This file is encoded using Unicode so as to support all languages. The structure of a card is thus:

```
====
QUESTION
---
ANSWER
---
```

Each cards starts with `====`, and ends at the next `====` or EOF. The QUESTION is the question, obviously, and accordingly, the ANSWER is the answer. The question and answer can be multiple lines long, ending at the `---`. Multiline question/answers are simply concatenated (`'\n'` replaced with `' '`) to one line.

