#include <stdlib.h>
#include <stdio.h>
#include "cache.h"
#include "lru.h"

void lru_init_queue (Set *set) {
  LRUNode *s = NULL;
  LRUNode **pp = &s;  // place to chain in the next node
  for (int i = 0; i < set->line_count; i++) {
    Line *line = &set->lines[i];
    LRUNode *node = (LRUNode *)(malloc(sizeof(LRUNode)));
    node->line = line;
    node->next = NULL;
    (*pp) = node;
    pp = &((*pp)->next);
  }
  set->lru_queue = s;
}

void lru_init (Cache *cache) {
  Set *sets = cache->sets;
  for (int i = 0; i < cache->set_count; i++) {
    lru_init_queue(&sets[i]);
  }
}

void lru_destroy (Cache *cache) {
  Set *sets = cache->sets;
  for (int i = 0; i < cache->set_count; i++) {
    LRUNode *p = sets[i].lru_queue;
    LRUNode *n = p;
    while (p != NULL ) {
      p = p->next;
      free(n);
      n = p;
    }
    sets[i].lru_queue = NULL;
  }
}

void lru_fetch(Set* set, unsigned int tag, LRUResult* result) {
  /* TODO:
     Implement the LRU algorithm to determine which line in
     the cache should be accessed.
   */

Line *line;
LRUNode *current;
current = set->lru_queue;


for (LRUNode **previous = &(set->lru_queue);
    (*previous) != NULL;
    previous = &((*previous)->next)){
  current = *previous;
  line = current->line;

  if(line->valid == 1 && line->tag == tag){ //if the line is valid and the tag matches
    result->access = HIT; //we have a hit; set the reuslt access to HIT
  }
  
  else if(line->valid == 0){ //else if the line is not valid
    line->valid = 1; //set the line valid to 1
    line->tag = tag; //set the line tag to the tag
    result->access = COLD_MISS; //set the result access to COLD_MISS
  }

  else if(current->next == NULL){ //else if the line is the last line
    line->tag = tag; //set the line tag to the tag
    result->access = CONFLICT_MISS; //set the result access to CONFLICT_MISS
  }
 
    else continue; //else continute to the next element

  *previous = current->next; //unchain current from the list; place it at the front
  current->next = set->lru_queue;
  set->lru_queue = current;
  result->line = line; //set the result line to the line

  return;
}
}
