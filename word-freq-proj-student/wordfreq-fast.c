#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hw.h"

/**
* The main entry point of the wordfreq-fast program.
*/

int main (int argc, char *argv[]) {
  // First determine if the output should be quiet:
  int quiet = 0;
  if (argc == 2 && strcmp(argv[1], "quiet") == 0) {
    quiet = 1;
  }
  
  // Next, create the WordSet object:
  WordSet *ws = wset_new();
  
  char *word;
  while ((word = words_next_word())) {
    // Add it to the set:
    wset_fast_add(ws, word);
    free(word);
  }
  
  if (!quiet) {
    wset_print(ws);
  }
  
  wset_free(ws);
  return 0;
}
