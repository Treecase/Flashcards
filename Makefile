


flashcards : *.c *.h
	gcc *.c -lncursesw -Wall -g -o flashcards

clean :
	rm flashcards

