
# TODO proper Makefile


flashcards : src/*.c src/*.h
	gcc src/*.c -lncursesw -Wall -g -o flashcards

clean :
	rm flashcards

