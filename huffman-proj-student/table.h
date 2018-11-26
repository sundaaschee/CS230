#ifndef __TABLE_H
#define __TABLE_H
#include "tree.h"

typedef struct EncodeTable EncodeTable;


/**
 * Returns an encoding table given the huffman tree.
 */
EncodeTable *table_build (TreeNode *root);


/**
 * Frees the encoding table.
 */
void table_free(EncodeTable* etab);

/**
 * This function returns the huffman encoding for a given character `c` in the
 * provided encoding table. The format of the returned string is a sequence of
 * '1' and '0' characters representing the encoding.  The string is terminated
 * by a null character.
 *
 * It is the responsibility of the caller to deallocate the array of
 * characters that are returned.
 */
char *table_bit_encode (EncodeTable *etab, unsigned char c);


/**
 * This function is useful for debugging. It prints out the mapping from
 * characters to their huffman encoding.
 */
void table_print (EncodeTable *etab);

#endif
