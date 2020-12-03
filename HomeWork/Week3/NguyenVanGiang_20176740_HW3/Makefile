CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 

all: main

kplc: main.o
	${CC} main.o -o kplc

main.o: main.c
	${CC} ${CFLAGS} main.c


clean:
	rm -f *.o *~