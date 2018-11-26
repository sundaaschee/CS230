#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <string.h>
#include "hzip.h"

void usage() {
  printf("tableg <file.txt>\n");
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

  EncodeTable *etab = table_build(tree);
  if (etab == NULL) {
    printf("Could not build the table!");
    usage();
    exit(1);
  }  

  table_print(etab);

  table_free(etab);
  tree_free(tree);

  return 0;
}
