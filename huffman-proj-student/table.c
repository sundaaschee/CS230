/********************************************************************

 The table module manages the mapping from characters to their Huffman
 encoding.  It requires a properly constructed Huffman tree to be built before
 it is used.  The primary functions that are exported by this module include:

 EncodeTable *table_build (TreeNode *root);

   - Returns an encoding table from a properly constructed Huffman tree.

 void table_free (EncodeTable *etab);

   - Frees an encoding table.

 char *table_bit_encode (EncodeTable *etab, unsigned char c);

   - Returns a bit encoding of the given character `c` with the provided
     encoding table. The format of the array of characters that are returned
     is a sequence of '1' and '0' characters terminated by a null ('\0').
     Each '1' and '0' in the encoding string represents a path to a character
     in the tree.  We use this encoding to write bits to the compressed file.

 *******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "table.h"

#define NUMBER_OF_CHARS 256


/**
 * A BitArray represents the encoding of a particular character.  It is used
 * by this module to map a character `c` to an array of characters which will
 * be either a '1' or a '0'.  The `bits` string is terminated by a null
 * character.  The length `len` is the length of the bit array (excluding the
 * null).
 */
typedef struct BitArray BitArray;
struct BitArray {
  int  len;
  char *bits;
};


/**
 * The `bit_array_new` function returns a new bit array with no bits (suitable
 * for use at the root of the tree).
 */
static BitArray *bit_array_new () {
  BitArray *b = (BitArray *)(malloc(sizeof(BitArray)));
  b->len = 0;
  b->bits = (char *)(malloc(1));
  b->bits[0] = 0;
  return b;
}


/**
 * The `bit_array_new_with` function returns a new bit array from the given
 * bit array, appending one new bit.  It is the responsibility of the caller
 * to manage any deallocation.
 */
static BitArray *bit_array_new_with (BitArray *b, int bit) {
  assert((bit & 1) == bit);  // check that it fits in a bit
  BitArray *nb = (BitArray *)(malloc(sizeof(BitArray)));
  nb->len = b->len;
  nb->bits = (char *)(malloc(nb->len+2));
  memcpy(nb->bits, b->bits, b->len);
  nb->bits[(nb->len)++] = ('0' + bit);
  nb->bits[nb->len] = 0;
  return nb;
}


/**
 * The `bit_array_free` function deallocates the bit array object.
 */
static void bit_array_free (BitArray *b) {
  assert(b != NULL && b->bits != NULL);
  free(b->bits);
  free(b);
}


/**
 * An EncodeTable holds a mapping from characters to bit arrays.  It is used
 * during the encoding process to retrieve the bit representation of a
 * character for the Huffman coding.
 */
struct EncodeTable {
  BitArray *table[NUMBER_OF_CHARS];
};


/**
 * This function recursively constructs the encoding table.  It visits each
 * node in the tree generating the encoding.  If the tree node is an INTERNAL
 * node we assign a 1 bit to the left child of the tree and a 0 bit to the
 * right child of a tree.  If the tree is a LEAF node we add the mapping to
 * the encode table.  We create new bit arrays as we descend into the tree.
 */
static void rec_gen_table (EncodeTable *etab, TreeNode *node, BitArray *b) {

  // If we have an internal node we recursively descend into the tree.
  if (node->type == INTERNAL) {
    TreeNode *left  = node->left;
    TreeNode *right = node->right;

    // If the left child is not NULL we create a new bit array from the old
    // one, appending a 1 bit, and recursively follow the left branch.
    if (left != NULL) {
      BitArray *lb = bit_array_new_with(b, 1);
      rec_gen_table(etab, left, lb);
    }

    // If the right child is not NULL we create a new bit array from the old
    // one, append a 0 bit, and recursively follow the right branch.
    if (right != NULL) {
      BitArray *rb = bit_array_new_with(b, 0);
      rec_gen_table(etab, right, rb);
    }

    // We need to make sure to deallocate the intermediate bit array.
    bit_array_free(b);

  } else {
    // Leaf Node Case:

    // We finally reached a leaf node so we add its mapping into the encode
    // table.  Note that in the leaf case we do NOT free the BitArray.
    unsigned char ch = node->freq.c;
    etab->table[ch] = b;
  }
}


/**
 * This function returns the Huffman encoding for a given character `c` in the
 * provided encoding table.  The format of the returned string is a sequence
 * of '1' and '0' characters representing the encoding.  The string is
 * terminated by a null.
 *
 * It is the responsibility of the caller to deallocate the string that is
 * returned.
 */
char *table_bit_encode (EncodeTable *etab, unsigned char c) {
  // Retrieve the encoding for the character:
  BitArray *b = etab->table[c];

  // Make a copy of the encoding:
  char *e = (char *)(malloc(b->len+1));
  strcpy(e, b->bits);

  // Note that it is the responsibility of the caller to deallocate
  // the array of characters that we return.
  return e;
}


/**
 * This function is useful for debugging. It prints out the mapping from
 * characters to their Huffman encoding.
 */
void table_print (EncodeTable *etab) {
  for (int i = 0; i < NUMBER_OF_CHARS; i++) {
    unsigned char c = (unsigned char)i;
    BitArray *b = etab->table[c];
    if (b != NULL) {
      printf("%d '%c' = %s\n", c, c, b->bits);
    }
  }
}


/**
 * Returns an encoding table given the Huffman tree.
 */
EncodeTable *table_build (TreeNode *root) {
  // Allocate a new encoding table:
  EncodeTable *etab = (EncodeTable *)(malloc(sizeof(EncodeTable)));

  // Initialize each entry to NULL:
  for (int i = 0; i < NUMBER_OF_CHARS; i++) {
    etab->table[i] = NULL;
  }

  // Recursively construct the encoding table:
  rec_gen_table(etab, root, bit_array_new());

  // Return the constructed table:
  return etab;
}


/**
 * Frees the encoding table.
 */
void table_free (EncodeTable *etab) {

  // First free each of the bit arrays in the table.
  for (int i = 0; i < NUMBER_OF_CHARS; i++) {
    BitArray *b = etab->table[i];
    if (b != NULL) {
      bit_array_free(b);
    }
  }

  free(etab);
}
