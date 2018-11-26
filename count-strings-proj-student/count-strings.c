#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// defines the <string,count> pair struct

typedef struct entry {
  char *string;
  int   count;
} Entry;


// fills in the entries of the table with the strings
// being searched for and an initial count of 0; the i'th
// entry has information about the i'th string

void prepare_table (int n, char **strings, Entry *entries) {
  // TODO: Fill this in first.
  
  for(int i = 0; i < n; i++){
    entries[i].count = 0;
    entries[i].string = strings[i];
  }
  
}


// The maximum length of a single line of input

#define LINE_MAX_CHARS 1000

// This reads each line of input from the given input file stream and
// searches for occurrences of each string in the entries, incrementing
// the strings' counts of their occurrences.  Argument n indicates the
// number of entries (number of strings and counts).  LINE_MAX_CHARS
// is the longest line that this function will read as a single unit.
// Note that a given string may occur more than once in a given line,
// and its occurrences may overlap.  For example, string aba occurs
// twice in the input ababa.  The function returns once it reaches
// end-of-file on the input stream.

void count_occurrences (int n, FILE *file, Entry *entries) {
  // TODO: Fill this in last.

  char umass[LINE_MAX_CHARS];

  while(fgets(umass, LINE_MAX_CHARS, file) != NULL){
    for(int i = 0; i < n; i++){
      char *go = strstr(umass, entries[i].string);
      while(go != NULL){
	entries[i].count++;
	//  go = go + 1;
	go = strstr(go+1, entries[i].string);
	
      }
    }
  }
  
}


// This prints the n entries in order.  For each entry it prints
// the entry's string, then a colon and a space, then the entry's count
// (in decimal), then a newline.  All printing is to standard output.

void print_occurrences (int n, Entry *entries) {
  // TODO: Fill this in second.
  for(int i = 0; i < n; i++){
    printf("%s: %d\n", entries[i].string, entries[i].count);
	 }
  
}


// The most strings this program will search for (more are ignored)

#define MAX_STRINGS 50

int main (int argc, char * argv[]) {
  if (argc < 2) {
    printf("Usage: %s s1 s2 s3 ...\n", argv[0]);
    printf("  Reads text from standard input and counts how many times\n");
    printf("  each string s1, s2, etc., occurs in the standard input text.\n");
    printf("  The program will consider up to 50 strings to search for;\n");
    printf("  any more than that wll be ignored.  Matches must be exact,\n");
    printf("  including upper/lower case.  Lines read from the input\n");
    printf("  are expected to be no more than %d characters long.\n", LINE_MAX_CHARS);
    printf("Example output:\n");
    printf("  If the invocation is \"%s my friend flicka\"\n", argv[0]);
    printf("  the output may look like:\n");
    printf("my: 17\n");
    printf("friend: 0\n");
    printf("flicka: 4\n");
    return 1;
  }
  int n = argc - 1;
  if (n > MAX_STRINGS) {
    n = MAX_STRINGS;
  }
  Entry entries[MAX_STRINGS];
  char **strings = argv+1;
  prepare_table(n, strings, entries);
  count_occurrences(n, stdin, entries);
  print_occurrences(n, entries);
}
