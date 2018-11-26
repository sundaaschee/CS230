CC = gcc
CFLAGS = --std=c99 -Wall -g
OBJS = bank.o atm.o command.o trace.o errors.o

all: banksim twriter treader

banksim: banksim.c $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

twriter: twriter.c $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

treader: treader.c $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

bank.o: bank.c bank.h
	$(CC) $(CFLAGS) -c bank.c

atm.o: atm.c atm.h
	$(CC) $(CFLAGS) -c atm.c

command.o: command.c command.h
	$(CC) $(CFLAGS) -c command.c

trace.o: trace.c trace.h
	$(CC) $(CFLAGS) command.o -c trace.c

errors.o: errors.c errors.h
	$(CC) $(CFLAGS) -c errors.c

test: buildtest
	CK_DEFAULT_TIMEOUT=15 bash -c './test/public-test'

buildtest: all test/public-test.c
	make -C test

clean:
	rm -f *.o banksim twriter treader
	make -C test clean

tar:
	tar czvf bank-submit.tgz *.c *.h
