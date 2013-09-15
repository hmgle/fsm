.PHONY: clean

all: fsm.o

%.o: %.c
	gcc $< -c -o $@ -Wall

clean:
	-rm -f *.o
