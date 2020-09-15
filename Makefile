#This is the Makefile for matrix multplication 

all : matrixMult.o rtclock.o
	gcc -Wall -o mmm main.c matrixMult.o rtclock.o -lpthread

matrixMult.o : matrixMult.c matrixMult.h
	gcc -Wall -c matrixMult.c

rtclock.o: rtclock.c rtclock.h
	gcc -Wall -c rtclock.c

clean: 
	rm -f mmm *.o 