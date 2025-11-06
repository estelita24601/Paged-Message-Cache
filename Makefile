CC = gcc
CFLAGS = -Wall
SRC_FILES = src/message.c src/cache.c
MSG_TEST_FILES =  $(SRC_FILES) tests/message_test.c
CACHE_TEST_FILES =  $(SRC_FILES) tests/cache_test.c


.PHONY: all main test_msg test_cache test clean
.DEFAULT_TARGET := all

all: main

main: $(SRC_FILES)
	${CC} ${CFLAGS} $^ -o main
	./message

# all: messages.c
# 	${CC} ${CFLAGS} $^ -o message
# 	./message

test_msg: $(MSG_TEST_FILES)
	${CC} ${CFLAGS} $^ -o msg_test -lm
	./msg_test

test_cache: $(CACHE_TEST_FILES)
	${CC} ${CFLAGS} $^ -o cache_test -lm
	./cache_test

test:  $(SRC_FILES) tests/tests.c
	${CC} ${CFLAGS} $^ -o test -lm
	./test

# run: all

clean:
	rm -f *.o
	rm -f main