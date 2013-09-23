.PHONY: clean

CFLAGS = -g -Wall -Wextra  -DDEBUG=1

all: test

test: test.o fsm.o

mem_test: all
	valgrind --leak-check=full ./test

clean:
	-rm -f *.o test
