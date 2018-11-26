CC = gcc
CFLAGS = --std=c99 -Wall -g
OBJS = tree.o pqueue.o huffman.o bits-io.o table.o decoder.o encoder.o

all: huffc huffd treeg tableg

huffc: $(OBJS) huffc.o
	$(CC) $(CFLAGS) $(OBJS) huffc.o -o huffc

huffd: $(OBJS) huffd.o
	$(CC) $(CFLAGS) $(OBJS) huffd.o -o huffd

treeg: $(OBJS) treeg.o
	$(CC) $(CFLAGS) $(OBJS) treeg.o -o treeg

tableg: $(OBJS) tableg.o
	$(CC) $(CFLAGS) $(OBJS) tableg.o -o tableg

huffc.o: huffc.c
	$(CC) $(CFLAGS) -c huffc.c

huffd.o: huffd.c
	$(CC) $(CFLAGS) -c huffd.c

treeg.o: treeg.c
	$(CC) $(CFLAGS) -c treeg.c

tableg.o: tableg.c
	$(CC) $(CFLAGS) -c tableg.c

tree.o: tree.c tree.h
	$(CC) $(CFLAGS) -c tree.c

pqueue.o: pqueue.c pqueue.h
	$(CC) $(CFLAGS) -c pqueue.c

bits-io.o: bits-io.c bits-io.h
	$(CC) $(CFLAGS) -c bits-io.c

huffman.o: huffman.c huffman.h
	$(CC) $(CFLAGS) -c huffman.c

table.o: table.c table.h
	$(CC) $(CFLAGS) -c table.c

decoder.o: decoder.c decoder.h
	$(CC) $(CFLAGS) -c decoder.c

encoder.o: encoder.c encoder.h
	$(CC) $(CFLAGS) -c encoder.c

test: buildtest
	CK_DEFAULT_TIMEOUT=15 bash -c './test/public-test'

buildtest: all test/public-test.c
	make -C test

clean:
	rm -f *.o
	rm -f huffc huffd tableg treeg
	make -C test clean

tar:
	tar czvf huffman-proj-student-submit.tgz *.c *.h

