CC = gcc
CFLAGS = -Wall
TEST_EXECUTABLE = msg_test

.PHONY: clean

all: test_message.c message.o
	$(CC) $(CFLAGS) $^ -o $(TEST_EXECUTABLE) -lm

message.o: message.c message.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o
	rm -f $(TEST_EXECUTABLE)