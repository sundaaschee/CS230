#ifndef __ENCODER_H
#define __ENCODER_H

/**
 * The Encoder structure is used to maintain all the information required to
 * encode an input file using the Huffman coding algorithm.
 */
typedef struct Encoder Encoder;


/**
 * Returns a pointer to an Encoder object or NULL if there is an error.
 */
Encoder *encoder_new (const char *infile, const char *outfile);


/**
 * Deallocates an Encoder object. Returns -1 if there is an error.
 */
int encoder_free (Encoder *encoder);


/**
 * Encodes the input file into the output file. Returns the number of bytes
 * encoded or -1 if there was an error.
 */
int encoder_encode (Encoder *encoder);

#endif
