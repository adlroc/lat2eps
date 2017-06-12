CC = gcc

CFLAGS = -Wall -O3
LFLAGS = 

all: lat2eps test

clean:
	rm -f lat2eps test *.o

lat2eps:	lat2eps_cmd.o lat2eps_lib.o
	$(CC) $(CFLAGS) $(LFLAGS) lat2eps_cmd.o lat2eps_lib.o -o lat2eps

test:	test.o lat2eps_lib.o
	$(CC) $(CFLAGS) $(LFLAGS) test.o lat2eps_lib.o -o test

lat2eps_cmd.o:	lat2eps_cmd.c lat2eps.h
	$(CC) $(CFLAGS) -c lat2eps_cmd.c -o lat2eps_cmd.o

test.o:	test.c lat2eps.h
	$(CC) $(CFLAGS) -c test.c -o test.o

lat2eps_lib.o:	lat2eps_lib.c lat2eps.h
	$(CC) $(CFLAGS) -c lat2eps_lib.c -o lat2eps_lib.o

