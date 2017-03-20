# To run, enter make or make all

all: test

test: main.o utils.o
	gcc  -lpthread -o test main.o utils.o

utils.o: utils.c utils.h
	gcc -c utils.c

main.o: main.c utils.h
	gcc -lpthread -c main.c
