CC = gcc
CFLAGS = -pthread -Wall -g
LDFLAGS = -pthread -Wall -g
OBJS = csapp.o

all: client

csapp.o: csapp.c csapp.h
	$(CC) $(CFLAGS) -c csapp.c	

client.o: client.c csapp.h
	$(CC) $(CFLAGS) -c client.c

client: client.o csapp.o

clean:
	rm -f *.o
	rm -f * client
