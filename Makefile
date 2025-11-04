CC = gcc
CFLAGS = -Wall
SRC_FILES = src/messages.c src/bst.c
TEST_FILES =  $(SRC_FILES) tests/tests.c


.PHONY: all main test clean
.DEFAULT_TARGET := all

all: main

main: $(SRC_FILES)
	${CC} ${CFLAGS} $^ -o main
	./message

# all: messages.c bst.c
# 	${CC} ${CFLAGS} $^ -o message
# 	./message

# test: messages.c bst.c tests.c
# 	${CC} ${CFLAGS} $^ -o tests
# 	./tests

test:  $(TEST_FILES)
	${CC} ${CFLAGS} $^ -o tests
	./tests

# run: all

clean:
	rm -f *.o
	rm -f main