
Flashcards
==========


This is a simple flashcard program. It makes use of a simple spaced repetition algorithm to improve retention. It also uses a simple-to-edit file format for card decks. Included are some example decks (Japanese Hiragana, Katakana, and numbers pronunciations).

The grammar for the deckfile is
```
( "Q:" [^;]* ';' [ \t\n]* "A:" [^;]* ';' )+
```

