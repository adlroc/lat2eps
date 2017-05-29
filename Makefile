CC = gcc

CFLAGS = -Wall -std=gnu99 -O3
LFLAGS = 

all: lat2eps

clean:
	rm -f lat2eps *.o

lat2eps:	lat2eps.c
	$(CC) $(CFLAGS) $(LFLAGS) lat2eps.c -o lat2eps

