CC = gcc
CFLAGS = -Wall

.PHONY: all test run clean
.DEFAULT_TARGET := all

all: messages.c bst.c
	${CC} ${CFLAGS} $^ -o message
	./message

test: messages.c bst.c tests.c
	${CC} ${CFLAGS} $^ -o tests
	./tests

run: all

clean:
	rm -f *.o
	rm -f *.txt
	rm -f tests