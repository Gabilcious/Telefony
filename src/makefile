CC = gcc
CFLAGS = -Werror -Wall -Wextra -std=c11 -O2

all: main.o phone_forward.o forwardTree.o reverseTree.o list.o
	$(CC) main.o phone_forward.o forwardTree.o reverseTree.o list.o -o main

main.o: main.c phone_forward.h
	$(CC) main.c -c -o main.o $(CFLAGS)

phone_forward.o: phone_forward.c phone_forward.h forwardTree.h reverseTree.h list.h
	$(CC) phone_forward.c -c -o phone_forward.o $(CFLAGS)

forwardTree.o: forwardTree.c forwardTree.h
	$(CC) forwardTree.c -c -o forwardTree.o $(CFLAGS)

reverseTree.o: reverseTree.c reverseTree.h list.h
	$(CC) reverseTree.c -c -o reverseTree.o $(CFLAGS)

list.o: list.c list.h
	$(CC) list.c -c -o list.o $(CFLAGS)

clean:
	@rm -f *.o main
