#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <string.h>
#include "hzip.h"

void usage() {
  printf("huffd <file.he> <file.txt>\n");
}


int main (int argc, char *argv[]) {
  if (argc != 3) {
    usage();
    exit(1);
  }

  // Get a reference to the input and output files:
  char *infile  = argv[1];
  char *outfile = argv[2];

  // Create a new decoder:
  Decoder *decoder = decoder_new(infile, outfile);
  if (decoder == NULL) {
    printf("decoder was null\n");
    usage();
    exit(1);
  }

  // Decode the file:
  decoder_decode(decoder);

  // Free up resources:
  decoder_free(decoder);

  return 0;
}
