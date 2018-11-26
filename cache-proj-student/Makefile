OBJS = src/cpu.o src/cache.o src/lru.o src/bits.o

SOLNBITS = src/soln-bits.o

CACHESIM = src/main.o

TESTSIM  = src/test-sim.o

%.o: %.c
	gcc -std=c99 -g -c $? -o $@

all: src/cache-sim src/test-sim

src/cache-sim: $(OBJS) $(CACHESIM)
	gcc $(OBJS) $(CACHESIM) -o cache-sim -lm

src/test-sim: $(OBJS) $(TESTSIM)
	gcc $(OBJS) $(SOLNBITS) $(TESTSIM) -o test-sim -lm

clean:
	rm -f $(OBJS) $(CACHESIM) $(TESTSIM) src/*~
	rm -f cache-sim test-sim

tar:
	tar czvf cache-proj-submit.tgz src/*.c src/*.h
