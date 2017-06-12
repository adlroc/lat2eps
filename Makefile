CC = gcc
LD = gcc
AR = ar

CFLAGS = -Wall -O3
LFLAGS = -L. -llat2eps

all: liblat2eps.a lat2eps test

clean:
	rm -f *.o

cleaner:
	rm -f liblat2eps.a lat2eps test *.o

liblat2eps.a:	lat2eps_lib.o
	$(AR) rcs liblat2eps.a lat2eps_lib.o

lat2eps:	lat2eps_cmd.o liblat2eps.a
	$(LD) $(LFLAGS) lat2eps_cmd.o -o lat2eps

test:	test.o liblat2eps.a
	$(LD) $(LFLAGS) test.o -o test

lat2eps_lib.o:	lat2eps_lib.c lat2eps.h
	$(CC) $(CFLAGS) -c lat2eps_lib.c -o lat2eps_lib.o

lat2eps_cmd.o:	lat2eps_cmd.c lat2eps.h
	$(CC) $(CFLAGS) -c lat2eps_cmd.c -o lat2eps_cmd.o

test.o:	test.c lat2eps.h
	$(CC) $(CFLAGS) -c test.c -o test.o

