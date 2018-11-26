#include "cache.h"
#include "cpu.h"
#include "bits.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

// Include these definitions to test against solution:
int soln_get_set  (Cache *cache, address_type address);
int soln_get_line (Cache *cache, address_type address);
int soln_get_byte (Cache *cache, address_type address);

int ntests = 24; // the number of tests
int passed = 0;  // the number of tests passed
int ntotal = 70; // the total number of points
int points = 0;  // the total number of points achieved

void score () {
  printf("====================================================\n");
  printf("You passed %d out of %d tests.\n", passed, ntests);
  printf("You scored %d/%d points.\n", points, ntotal);
  if (points == ntotal) {
    printf("Nice Work!\n");
  }
  printf("====================================================\n");
}

void test (int pred, char *success, char *fail, int tnum, int pts) {
  // check test predicate
  if (pred) {
    passed++;
    printf("Test %d PASSED: %s\n", tnum, success);
    points += pts;
  } else {
    printf("Test %d FAILED: %s\n", ntests, fail);
    score();
    //exit(1);
  }
}

char *msg (const char *fmt, ...) {
  char *buf = malloc(1024);  // we don't care about deallocation here!
  va_list ap;
  va_start(ap, fmt);
  vsprintf(buf, fmt, ap);
  return buf;
}

void test_cache   ();
void test_bits    ();
void test_results ();

int main (int argc, char *argv[]) {
  test_cache();
  test_bits();
  test_results();
  score();
}

void test_cache () {
  printf("====================================================\n");
  int sets  = 4;
  int lines = 4;
  int bytes = 10;
  Cache *cache = make_cache(sets, lines, bytes);

  test(cache != NULL,
       "cache is not NULL",
       "cache is NULL",
       1,
       0);
  
  // Test the basic initialization of the cache
  test(sets == cache->set_bits,
       "cache->set_bits properly initialized",
       msg("cache->set_bits improperly initialized: expected %d", sets),
       2,
       4);

  test(bytes == cache->block_bits,
       "cache->block_bits properly initialized",
       msg("cache->block_bits improperly initialized: expected %d", bytes),
       3,
       4);

  test(lines == cache->line_count,
       "cache->line_count properly initialized",
       msg("cache->line_count improperly initialized: expected %d", lines),
       4,
       2);

  // Test the proper initialization of the set and block count (10 points)
  int set_count      = exp2(sets);
  int block_size     = exp2(bytes);

  test(set_count == cache->set_count,
       "cache->set_count properly initialized",
       msg("cache->set_count improperly initialized: expected %d", set_count),
       5,
       3);

  test(block_size == cache->block_size,
       "cache->block_size properly initialized",
       msg("cache->block_size improperly initialized: expected %d", block_size),
       6,
       3);

  // Test cache set lists
  Set *set_list = cache->sets;
  test(set_list != NULL,
       "cache->sets is not NULL",
       msg("cache->sets should not be NULL"),
       7,
       3);

  int testval = 0;
  for (int i = 0; i < cache->set_count; i++) {
    testval |= (set_list[i].line_count != lines);
  }

  test(!testval,
       "line_count in Set is properly initialized",
       msg("line_count in Set is properly initialized; expected (%d)", lines),
       8,
       1);


  // Test LRU queues
  testval = 0;
  for (int i = 0; i < cache->set_count; i++) {
    testval |= (set_list[i].lru_queue == NULL);
  }

  test(!testval,
       "Your LRU queues have been properly initialized",
       "Your LRU queues have been improperly initialized",
       9,
       1);

  LRUNode *lru_p = NULL;
  int lru_c;
  testval = 0;
  for (int i = 0; i < cache->set_count; i++) {
    lru_c = 0;
    lru_p = set_list[i].lru_queue;
    while (lru_p != NULL) {
      lru_c++;
      lru_p = lru_p->next;
    }
    testval |= (lru_c != lines);
  }

  test(!testval,
       msg("Your LRU queues appear to have the correct number of nodes (%d)", lines),
       msg("Your LRU queues appear to have an incorrect number of nodes (%d); expected (%d)", lru_c, lines),
       10,
       3);

  // Test lines
  testval = 0;
  for (int i = 0; i < cache->set_count; i++) {
    Line *lines = set_list[i].lines;
    testval |= (lines == NULL);
  }

  test(!testval,
       "lines in Set is not NULL",
       "lines in Set is NULL",
       11,
       2);

  testval = 0;
  for (int i = 0; i < cache->set_count; i++) {
    Line *lines = set_list[i].lines;
    for (int j = 0; j < cache->line_count; j++) {
      testval |= (lines[j].accessed == NULL || lines[j].block_size != block_size);
    }
  }

  test(!testval,
       "bytes and block_size in Line are properly initialized",
       "bytes and block_size in Line are not properly initialized",
       12,
       2);
}

int test_read_address (CPU *cpu, TraceLine *trace_line) {
  return fscanf(cpu->address_trace, "%c %x,%c\n",
                &trace_line->operation,
                &trace_line->address,
                &trace_line->size);
}

void test_bits () {
  int sets  = 4;
  int lines = 4;
  int bytes = 10;
  Cache *cache = make_cache(sets, lines, bytes);
  CPU   *cpu   = make_cpu(cache, "traces/cat.trace");

  printf("====================================================\n");
  TraceLine trace_line;
  int setval   = 0;
  int tagval   = 0;
  int blockval = 0;
  unsigned int s;
  unsigned int t;
  unsigned int b;
  unsigned int ss;
  unsigned int st;
  unsigned int sb;
  while (test_read_address(cpu, &trace_line) != EOF) {
    s  = get_set(cache, trace_line.address);
    t  = get_line(cache, trace_line.address);
    b  = get_byte(cache, trace_line.address);
    ss = soln_get_set(cache, trace_line.address);
    st = soln_get_line(cache, trace_line.address);
    sb = soln_get_byte(cache, trace_line.address);
    if (s != ss || t != st || b != sb) {
      break;
    }
  }

  test(s == ss,
      "get_set is returning the proper set value",
      msg("get_set is not returning the proper set value; returned %d; expected %d",
          s, ss),
      13,
      2);

  test(t == st,
      "get_line is returning the proper tag value",
      msg("get_line is not returning the proper tag value; returned %d; expected %d",
          t, st),
      14,
      2);

  test(b == sb,
      "get_byte is returning the proper byte index",
      msg("get_byte is not returning the proper byte index; returned %d; expected %d",
          b, sb),
      15,
      2);
}

int sim_test_pts = 0;

int nearness (double v1, double v2, int tot) {
  return (v1/v2) * tot;
}

void test_cat_trace () {
  int sets  = 4;
  int lines = 4;
  int bytes = 10;
  Cache *cache = make_cache(sets, lines, bytes);
  CPU   *cpu   = make_cpu(cache, "traces/cat.trace");
  run_cpu(cpu);

  test(cpu->cold+cpu->conflict == 1354,
       msg("the number of misses (cpu->cold+cpu->conflict) is correct (%d)", cpu->cold+cpu->conflict),
       msg("the number of misses (cpu->cold+cpu->conflict) is incorrect was %d; expected %d", 
	   cpu->cold+cpu->conflict, 1354),
       16,
       nearness(cpu->cold+cpu->conflict, 1354, 4));

  test(cpu->hits == 803213,
       msg("the number of hits (cpu->hits) is correct (%d)", cpu->hits),
       msg("the number of hits (cpu->hits) is incorrect was %d; expected %d", 
	   cpu->hits, 803213),
       17,
       nearness(cpu->hits, 803213, 4));

  test(cpu->conflict == 1290,
       msg("the number of evictions (cpu->conflict) is correct (%d)", cpu->conflict),
       msg("the number of hits (cpu->conflict) is incorrect was %d; expected %d", 
	   cpu->conflict, 1290),
       18,
       nearness(cpu->conflict, 1290, 4));

  /* float misses_p   = ((float)(cpu->cold+cpu->conflict))/((float)1354) * 4; */
  /* float hits_p     = ((float)(cpu->hits))/((float)803213) * 4; */
  /* float conflict_p = ((float)(cpu->conflict))/((float)1290) * 4; */
  /* (int)(misses_p+hits_p+conflict_p); */
}


void test_ls_trace () {
  int sets  = 4;
  int lines = 1;
  int bytes = 5;
  Cache *cache = make_cache(sets, lines, bytes);
  CPU*   cpu   = make_cpu(cache, "traces/ls.trace");
  run_cpu(cpu);

  test(cpu->cold+cpu->conflict == 421934,
       msg("the number of misses (cpu->cold+cpu->conflict) is correct (%d)", cpu->cold+cpu->conflict),
       msg("the number of misses (cpu->cold+cpu->conflict) is incorrect was %d; expected %d", 
	   cpu->cold+cpu->conflict, 421934),
       19,
       nearness(cpu->cold+cpu->conflict, 421934, 4));

  test(cpu->hits == 2016836,
       msg("the number of hits (cpu->hits) is correct (%d)", cpu->hits),
       msg("the number of hits (cpu->hits) is incorrect was %d; expected %d", 
	   cpu->hits, 2016836),
       20,
       nearness(cpu->hits, 2016836, 4));

  test(cpu->conflict == 421918,
       msg("the number of evictions (cpu->conflict) is correct (%d)", cpu->conflict),
       msg("the number of hits (cpu->conflict) is incorrect was %d; expected %d", 
	   cpu->conflict, 421918),
       21,
       nearness(cpu->conflict, 421918, 4));

  /* float misses_p   = ((float)(cpu->cold+cpu->conflict))/((float)421934) * 4; */
  /* float hits_p     = ((float)(cpu->hits))/((float)2016836) * 4; */
  /* float conflict_p = ((float)(cpu->conflict))/((float)421918) * 4; */
  /* sim_test_pts += (int)(misses_p+hits_p+conflict_p); */
}

void test_wc_trace () {
  int sets  = 2;
  int lines = 4;
  int bytes = 8;
  Cache *cache = make_cache(sets, lines, bytes);
  CPU   *cpu   = make_cpu(cache, "traces/wc.trace");
  run_cpu(cpu);

  test(cpu->cold+cpu->conflict == 22104,
       msg("the number of misses (cpu->cold+cpu->conflict) is correct (%d)", cpu->cold+cpu->conflict),
       msg("the number of misses (cpu->cold+cpu->conflict) is incorrect was %d; expected %d", 
	   cpu->cold+cpu->conflict, 22104),
       22,
       nearness(cpu->cold+cpu->conflict, 22104, 4));

  test(cpu->hits == 787666,
       msg("the number of hits (cpu->hits) is correct (%d)", cpu->hits),
       msg("the number of hits (cpu->hits) is incorrect was %d; expected %d", 
	   cpu->hits, 787666),
       23,
       nearness(cpu->hits, 787666, 4));

  test(cpu->conflict == 22088,
       msg("the number of evictions (cpu->conflict) is correct (%d)", cpu->conflict),
       msg("the number of hits (cpu->conflict) is incorrect was %d; expected %d", 
	   cpu->conflict, 22088),
       24,
       nearness(cpu->conflict, 22088, 4));

  /* float misses_p   = ((float)(cpu->cold+cpu->conflict))/((float)22104) * 4; */
  /* float hits_p     = ((float)(cpu->hits))/((float)787666) * 4; */
  /* float conflict_p = ((float)(cpu->conflict))/((float)22088) * 4; */
  /* sim_test_pts += (int)(misses_p+hits_p+conflict_p); */
}

void test_results () {
  printf("====================================================\n");
  test_cat_trace();
  test_ls_trace();
  test_wc_trace();
}
