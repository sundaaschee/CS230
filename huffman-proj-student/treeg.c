#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <string.h>
#include "hzip.h"

void usage() {
  printf("treeg <file.txt>\n");
}

int main (int argc, char *argv[]) {
  if (argc != 2) {
    usage();
    exit(1);
  }

  char *infile = argv[1];

  TreeNode *tree = huffman_build_tree(infile);
  if (tree == NULL) {
    printf("Could not build the tree!");
    usage();
    exit(1);
  }

  tree_print(tree);
  tree_free(tree);

  return 0;
}
