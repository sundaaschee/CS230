#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <string.h>
#include "hzip.h"

void usage() {
  printf("huffc <file.txt> <file.he>\n");
}

int main (int argc, char *argv[]) {
  if (argc != 3) {
    usage();
    exit(1);
  }

  char *infile  = argv[1];
  char *outfile = argv[2];

  int result;

  Encoder *encoder = encoder_new(infile, outfile);
  if (encoder == NULL) {
    printf("Encoder failed to initialize.");
    exit(1);
  }

  result = encoder_encode(encoder);
  if (result == -1) {
    printf("Problem occurred during encoding.");
    exit(1);
  }

  result = encoder_free(encoder);
  if (result == -1) {
    printf("Encoder failed to free properly.");
  }

  return 0;
}
