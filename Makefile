CC = gcc
CFLAGS = -Wall
SRC_FILES = src/message.c src/cache.c tests/helpers.c
MSG_TEST_FILES =  $(SRC_FILES) tests/message_test.c
CACHE_TEST_FILES =  $(SRC_FILES) tests/cache_test.c
PAGE_TEST_FILES = ${SRC_FILES} tests/page_test.c

#make cache size smaller for testing
TEST_CONFIG = -DCACHE_SIZE=4

.PHONY: all main test_msg test_cache test_page test clean
.DEFAULT_TARGET := all

#all: main
#
#main: $(SRC_FILES)
#	${CC} ${CFLAGS} $^ -o main
#	./message

# all: messages.c
# 	${CC} ${CFLAGS} $^ -o message
# 	./message

test_msg: $(MSG_TEST_FILES)
	${CC} ${CFLAGS} $^ -o msg_test -lm
	./msg_test

test_cache: $(CACHE_TEST_FILES)
	${CC} ${CFLAGS} ${TEST_CONFIG} $^ -o cache_test -lm
	./cache_test

test_page: $(PAGE_TEST_FILES)
	$(CC) $(CFLAGS) ${TEST_CONFIG} $^ -o page_test -lm
	./page_test

test:  $(SRC_FILES) tests/tests.c
	${CC} ${CFLAGS} $^ -o test -lm
	./test

# run: all

clean:
	rm -f *.o
	rm -f main
	rm -f *test