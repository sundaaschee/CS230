#ifndef __HUFFMAN_H
#define __HUFFMAN_H
#include "tree.h"

/**
 * Returns a pointer to a TreeNode object or NULL if there is an error.
 * 
 * Given a filename this function will use the Huffman coding algorithm to
 * construct a Huffman tree that can be used to encode characters from the
 * input file.
 */
TreeNode *huffman_build_tree (const char *filename);

/**
 * Returns the character for the given encoding string or -1 on error.
 *
 * (See table.c for details on the encoding string.)
 *
 */
int huffman_find (TreeNode *root, char *encoding);

#endif
