.PHONY: clean

CFLAGS = -g -Wall -Wextra

all: test

test: test.o fsm.o

clean:
	-rm -f *.o test
