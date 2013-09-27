.PHONY: clean

CFLAGS = -g -Wall -DDEBUG=1

all: test test2

test: test.o fsm.o

test2: test2.o fsm.o

mem_test: all
	valgrind --leak-check=full ./test

clean:
	-rm -f *.o test
