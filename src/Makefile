CC = gcc
CFLAGS = -Wall

.PHONY: clean
.DEFAULT_TARGET := all

all: main

main: main.c
	${CC} ${CFLAGS} $^ -o main

# all: messages.c bst.c
# 	${CC} ${CFLAGS} $^ -o message
# 	./message

# test: messages.c bst.c tests.c
# 	${CC} ${CFLAGS} $^ -o tests
# 	./tests

# run: all

clean:
	rm -f *.o
	rm -f main