#ifndef __DECODER_H
#define __DECODER_H

/**
 * The Decoder structure is used to maintain all the information
 * required to decode an input file using the huffman coding
 * algorithm.
 */
typedef struct Decoder Decoder;

/**
 * Returns a pointer to an Decoder object or NULL if there is an
 * error.
 */
Decoder *decoder_new (const char *infile, const char *outfile);

/**
 * Deallocates an Decoder object. Returns -1 if there is an error.
 */
int decoder_free (Decoder *decoder);

/**
 * Decodes the input file to the output file.
 */
void decoder_decode (Decoder *decoder);

#endif
