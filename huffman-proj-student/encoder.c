/********************************************************************
 
 The encoder module is the main API for the Huffman encoding functionality.

*******************************************************************/

#include <stdlib.h>
#include <assert.h>
#include "table.h"
#include "tree.h"
#include "huffman.h"
#include "bits-io.h"
#include "encoder.h"

/**
 * The Encoder structure is used to maintain all the information required to
 * encode an input file using the Huffman coding algorithm.
 */
struct Encoder {
  FILE        *infile;  // The file we are reading in
  int          bytes;   // Number of uncompresed bytes read/written
  TreeNode    *tree;    // The Huffman tree
  EncodeTable *etab;    // The encoding table
  BitsIOFile  *bfile;   // The bits-io file we are writing to
};


/**
 * Returns a pointer to an Encoder object or NULL if there is an error.
 * 
 * Given the name of an input file and output file we create and initialize an
 * Encoder object. When the Encoder object is returned it is ready for the
 * encoding of the input file to happen.
 */
Encoder *encoder_new (const char *infile, const char *outfile) {
  FILE *infp = fopen(infile, "r");
  if (infp == NULL) {
    return NULL;
  }

  TreeNode *tree = huffman_build_tree(infile);
  if (tree == NULL) {
    return NULL;
  }

  int count = tree_count(tree);

  EncodeTable *etab = table_build(tree);
  if (etab == NULL) {
    return NULL;
  }

  BitsIOFile *bfile = bits_io_open(outfile, "w");
  if (bfile == NULL) {
    return NULL;
  }

  Encoder *encoder = (Encoder *)(malloc(sizeof(Encoder)));
  encoder->infile = infp;
  encoder->bytes  = count;
  encoder->tree   = tree;
  encoder->etab   = etab;
  encoder->bfile  = bfile;

  return encoder;
}


/**
 * Deallocates an Encoder object. Returns -1 if there is an error.
 */
int encoder_free (Encoder *encoder) {
  assert(encoder != NULL);
  fclose(encoder->infile);
  tree_free(encoder->tree);
  table_free(encoder->etab);
  int res = bits_io_close(encoder->bfile);
  free(encoder);
  return res;
}


/**
 * Encodes the input file into the output file. Returns the number of bytes
 * encoded or -1 if there was an error.
 */
int encoder_encode (Encoder *encoder) {
  // First, we need to write the number of bytes that will be encoded,
  // to the output file:
  int r = bits_io_write_length(encoder->bfile, encoder->bytes);
  if (r == EOF)
    return -1;

  // Next, we need to write the tree to the output file:
  r = bits_io_write_tree(encoder->bfile, encoder->tree);
  if (r == EOF)
    return -1;

  // Now, we encode each of the characters from the input
  // file to the output file:
  int count = 0;
  for (int ch; (ch = fgetc(encoder->infile)) != EOF; ) {
    ++count;
    // Lookup the encoding for the character:
    char *estr = table_bit_encode(encoder->etab, ch);

    // Loop over the encoding writing the correct bits
    // to the output file:
    for (char c, *p = estr; (c = *p++); ) {
      if (bits_io_write_bit(encoder->bfile, c - '0') == EOF) {
	return -1;
      }
    }

    // We need to free the encoding string that is
    // returned from the table:
    free(estr);
  }

  return count;
}
