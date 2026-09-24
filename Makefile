CC     = gcc
CFLAGS = -Wall -Wextra -std=c11 -D_POSIX_C_SOURCE=200809L
LDLIBS = -lncurses

clock: clock.o numbers.o
	$(CC) $^ -o $@ $(LDLIBS)

clock.o: clock.c numbers.h
numbers.o: numbers.c numbers.h

clean:
	rm -f *.o clock