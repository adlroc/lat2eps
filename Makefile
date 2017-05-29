CC = clang
CPP = clang++

CFLAGS = -O3 -Wall
LFLAGS = 

SRCSC = $(wildcard *.c)

SRCSCPP = $(wildcard *.cpp)

PROGSC = $(patsubst %.c,%,$(SRCSC))

PROGSCPP = $(patsubst %.cpp,%,$(SRCSCPP))

all: $(PROGSC) $(PROGSCPP)

clean:
	rm -rf $(PROGSC) $(PROGSCPP) *.dSYM

%: %.c
	$(CC) $(CFLAGS)  -o $@ $<

%: %.cpp
	$(CPP) $(CFLAGS)  -o $@ $<

