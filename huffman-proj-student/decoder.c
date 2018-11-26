/********************************************************************
 
 The decoder module is the main API for the Huffman decoding
 functionality.

*******************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "tree.h"
#include "bits-io.h"
#include "decoder.h"

/**
 * The Decoder structure is used to maintain all the information required to
 * decode an input file using the Huffman coding algorithm.
 */
struct Decoder {
  FILE       *outfp;
  int         bytes;
  BitsIOFile *bfile;
  TreeNode   *tree;
};


/**
 * Returns a pointer to an Decoder object or NULL if there is an error.
 */
Decoder *decoder_new (const char *infile, const char *outfile) {
  assert (infile != NULL && outfile != NULL);

  // Open the file with bits-io library:
  BitsIOFile *bfile = bits_io_open(infile, "r");
  if (bfile == NULL)
    return NULL;

  // Open the output file:
  FILE *outfp = fopen(outfile, "w");
  if (outfp == NULL)
    return NULL;

  // Create the Decoder object:
  Decoder *decoder = (Decoder *)(malloc(sizeof(Decoder)));
  decoder->bfile   = bfile;
  decoder->outfp   = outfp;

  // Read the original file's length:
  int length = bits_io_read_length(bfile);
  if (length < 0) {
    bits_io_close(decoder->bfile);
    return NULL;
  }
  decoder->bytes = length;

  // Read the tree:
  TreeNode *tree = bits_io_read_tree(bfile);
  if (tree == NULL) {
    bits_io_close(decoder->bfile);
    return NULL;
  }
  decoder->tree = tree;

  return decoder;
}


/**
 * Deallocates an Decoder object. Returns -1 if there is an error.
 */
int decoder_free (Decoder *decoder) {
  assert(decoder != NULL);
  int status = 0;
  status = bits_io_close(decoder->bfile);
  tree_free(decoder->tree);
  fclose(decoder->outfp);
  free(decoder);
  return status;
}

/**
 * A utility function to decode a single character.
 */
static int decode_one (Decoder *decoder) {

  assert(decoder != NULL);

  if (decoder->bytes <= 0) {
    return EOF;
  }
  --(decoder->bytes);

  BitsIOFile *bfile = decoder->bfile;
  TreeNode   *tree  = decoder->tree;

  TreeNode *p = tree;
  while (p->type != LEAF) {
    int bit = bits_io_read_bit(bfile);
    if (bit == EOF) {
      return EOF;
    } else if (bit == 1) {
      p = p->left;
    } else {
      p = p->right;
    }

    if (p == NULL)
      return EOF;
  }

  return p->freq.c & 0xFF;
}


/**
 * Decodes the input file to the output file.
 */
void decoder_decode (Decoder *decoder) {

  assert(decoder != NULL);

  // Read characters:
  for (int ch; (ch = decode_one(decoder)) != EOF; ) {
    fprintf(decoder->outfp, "%c", ch);
  }
  return;
}
