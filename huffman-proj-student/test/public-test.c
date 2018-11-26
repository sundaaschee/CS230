#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
// Include the check header file:
#include <check.h>

// Include assignment header file:
#include "../hzip.h"
#include "../encoder.h"
#include "../decoder.h"

#define BIT_ITERATIONS 100007

// We include this definition here so we can gain access to the struct
// that is being returned from the bits-io module.
struct BitsIOFile {
  FILE *fp;
  char mode;
  char bitsLeft;
  unsigned char byte;
  int numBytes;
};

void setup() {
  // Perform any setup operations.
}

void teardown() {
  // Perform any teardown operations.
}

//  For more details see http://check.sourceforge.net

//////////////////////////////////////////////////////////////////////
///////////// bits-io unit tests
//////////////////////////////////////////////////////////////////////

START_TEST(test_bits_io_open)
{
  BitsIOFile *bfile = bits_io_open("books/aladdin.txt", "r");
  ck_assert_msg(bfile != NULL, "bfile should not be null.");
  ck_assert_msg(bfile->fp != NULL, "bfile->fp should not be null.");
  ck_assert_msg(bfile->mode == 'r', "bfile->mode should be 'r'.");
}
END_TEST

START_TEST(test_bits_io_close)
{
  BitsIOFile *bfile = bits_io_open("books/aladdin.txt", "r");
  ck_assert_msg(bfile != NULL, "bfile should not be null.");
  ck_assert_msg(bfile->fp != NULL, "bfile->fp should not be null.");
  ck_assert_msg(bfile->mode == 'r', "bfile->mode should be 'r'.");
  bits_io_close(bfile);
}
END_TEST

START_TEST(test_bits_io_write_bit)
{
  // Delete the files if they exist:
  remove("test/test.hf");
  remove("test/test-stress.hf");

  BitsIOFile *bfile = bits_io_open("test/test.hf", "w");
  unsigned char bit;
  bit = bits_io_write_bit(bfile, 1);
  ck_assert_int_eq(bit, 1);
  bit = bits_io_write_bit(bfile, 1);
  ck_assert_int_eq(bit, 1);
  bit = bits_io_write_bit(bfile, 0);
  ck_assert_int_eq(bit, 0);
  bit = bits_io_write_bit(bfile, 1);
  ck_assert_int_eq(bit, 1);
  bit = bits_io_write_bit(bfile, 0);
  ck_assert_int_eq(bit, 0);
  int result = bits_io_close(bfile);
  ck_assert_msg(result != EOF, "closing the file should not be EOF.");

  bfile = bits_io_open("test/test-stress.hf", "w");
  for (int i = 0; i < BIT_ITERATIONS; i++) {
    bit = bits_io_write_bit(bfile, i%2);
    ck_assert_int_eq(bit, i%2);
  }
  // count here is before the close; the close will add one more
  ck_assert_int_eq(bits_io_num_bytes(bfile), ((BIT_ITERATIONS+7)/8) - 1);

  result = bits_io_close(bfile);
  ck_assert_msg(result != EOF, "closing the file should not be EOF.");
}
END_TEST

START_TEST(test_bits_io_read_bit)
{
  BitsIOFile *bfile = bits_io_open("test/test.hf", "r");
  unsigned char bit;
  bit = bits_io_read_bit(bfile);
  ck_assert_int_eq(bit, 1);
  bit = bits_io_read_bit(bfile);
  ck_assert_int_eq(bit, 1);
  bit = bits_io_read_bit(bfile);
  ck_assert_int_eq(bit, 0);
  bit = bits_io_read_bit(bfile);
  ck_assert_int_eq(bit, 1);
  bit = bits_io_read_bit(bfile);
  ck_assert_int_eq(bit, 0);

  int nbytes = bits_io_num_bytes(bfile);
  ck_assert_int_eq(nbytes, 1);

  int result = bits_io_close(bfile);
  ck_assert_msg(result != EOF, "closing the file should not be EOF.");

  bfile = bits_io_open("test/test-stress.hf", "r");
  for (int i = 0; i < BIT_ITERATIONS; i++) {
    bit = bits_io_read_bit(bfile);
    ck_assert_int_eq(bit, i%2);
  }
  // count here includes any partially filled final byte
  ck_assert_int_eq(bits_io_num_bytes(bfile), (BIT_ITERATIONS+7)/8);

  result = bits_io_close(bfile);
  ck_assert_msg(result != EOF, "closing the file should not be EOF.");

  // Delete the files:
  remove("test/test.hf");
  remove("test/test-stress.hf");
}
END_TEST

//////////////////////////////////////////////////////////////////////
///////////// tree unit tests
//////////////////////////////////////////////////////////////////////

START_TEST(test_tree_new)
{
  TreeNode *t = tree_new();
  ck_assert_msg(t != NULL, "Tree node should not be NULL.");
}
END_TEST

START_TEST(test_tree_free)
{
  TreeNode *t = tree_new();
  ck_assert_msg(t != NULL, "Tree node should not be NULL.");

  tree_free(t);
}
END_TEST

START_TEST(test_tree_size)
{
  TreeNode *t = tree_new();
  t->left = tree_new();
  t->right = tree_new();
  ck_assert_int_eq(tree_size(t), 3);
  tree_free(t);
}
END_TEST

//////////////////////////////////////////////////////////////////////
///////////// pqueue unit tests
//////////////////////////////////////////////////////////////////////

START_TEST(test_pqueue_new)
{
  PriorityQueue *pq = pqueue_new();
  ck_assert_msg(pq != NULL, "Priority queue should not be NULL.");
}
END_TEST

START_TEST(test_pqueue_free)
{
  PriorityQueue *pq = pqueue_new();
  ck_assert_msg(pq != NULL, "Priority queue should not be NULL.");

  pqueue_free(pq);
}
END_TEST

START_TEST(test_pqueue_enqueue)
{
  PriorityQueue *pq = pqueue_new();
  ck_assert_msg(pq != NULL, "Priority queue should not be NULL.");

  pqueue_enqueue(pq, tree_new());
  ck_assert_int_eq(pqueue_size(pq), 1);
  pqueue_enqueue(pq, tree_new());
  ck_assert_int_eq(pqueue_size(pq), 2);
  pqueue_enqueue(pq, tree_new());
  ck_assert_int_eq(pqueue_size(pq), 3);
  pqueue_enqueue(pq, tree_new());
  ck_assert_int_eq(pqueue_size(pq), 4);

  pqueue_free(pq);
}
END_TEST

START_TEST(test_pqueue_dequeue)
{
  PriorityQueue *pq = pqueue_new();
  ck_assert_msg(pq != NULL, "Priority queue should not be NULL.");

  TreeNode *t = tree_new();
  t->type = LEAF;
  t->freq.v = 10;
  t->freq.c = 'c';
  t->left   = NULL;
  t->right  = NULL;

  pqueue_enqueue(pq, t);
  ck_assert_int_eq(pqueue_size(pq), 1);

  t = tree_new();
  t->type = LEAF;
  t->freq.v = 15;
  t->freq.c = 'x';
  t->left   = NULL;
  t->right  = NULL;

  pqueue_enqueue(pq, t);
  ck_assert_int_eq(pqueue_size(pq), 2);

  t = tree_new();
  t->type = LEAF;
  t->freq.v = 9;
  t->freq.c = 'q';
  t->left   = NULL;
  t->right  = NULL;

  pqueue_enqueue(pq, t);
  ck_assert_int_eq(pqueue_size(pq), 3);

  t = tree_new();
  t->type = LEAF;
  t->freq.v = 999;
  t->freq.c = 'a';
  t->left   = NULL;
  t->right  = NULL;

  pqueue_enqueue(pq, t);
  ck_assert_int_eq(pqueue_size(pq), 4);

  t = pqueue_dequeue(pq);
  ck_assert_int_eq(t->freq.c, 'q');
  ck_assert_int_eq(pqueue_size(pq), 3);
  free(t);

  t = pqueue_dequeue(pq);
  ck_assert_int_eq(t->freq.c, 'c');
  ck_assert_int_eq(pqueue_size(pq), 2);
  free(t);

  t = pqueue_dequeue(pq);
  ck_assert_int_eq(t->freq.c, 'x');
  ck_assert_int_eq(pqueue_size(pq), 1);
  free(t);

  t = pqueue_dequeue(pq);
  ck_assert_int_eq(t->freq.c, 'a');
  ck_assert_int_eq(pqueue_size(pq), 0);
  free(t);

  pqueue_free(pq);
}
END_TEST

//////////////////////////////////////////////////////////////////////
///////////// huffman unit tests
//////////////////////////////////////////////////////////////////////

START_TEST(test_huffman_build_tree)
{
  TreeNode *t;
  t = huffman_build_tree("books/aladdin.txt");
  ck_assert_msg(t != NULL, "Tree should not be NULL.");
  ck_assert_int_eq(tree_size(t), 115);
  free(t);

  t = huffman_build_tree("books/allbytes.txt");
  ck_assert_msg(t != NULL, "Tree should not be NULL.");
  ck_assert_int_eq(tree_size(t), 511);
  free(t);

  t = huffman_build_tree("books/holmes.txt");
  ck_assert_msg(t != NULL, "Tree should not be NULL.");
  ck_assert_int_eq(tree_size(t), 161);
  free(t);

  t = huffman_build_tree("books/iliad.txt");
  ck_assert_msg(t != NULL, "Tree should not be NULL.");
  ck_assert_int_eq(tree_size(t), 131);
  free(t);

  t = huffman_build_tree("books/KJV.txt");
  ck_assert_msg(t != NULL, "Tree should not be NULL.");
  ck_assert_int_eq(tree_size(t), 147);
  free(t);

  t = huffman_build_tree("books/newton.txt");
  ck_assert_msg(t != NULL, "Tree should not be NULL.");
  ck_assert_int_eq(tree_size(t), 315);
  free(t);

  t = huffman_build_tree("books/odyssy.txt");
  ck_assert_msg(t != NULL, "Tree should not be NULL.");
  ck_assert_int_eq(tree_size(t), 161);
  free(t);

  t = huffman_build_tree("books/poems.txt");
  ck_assert_msg(t != NULL, "Tree should not be NULL.");
  ck_assert_int_eq(tree_size(t), 261);
  free(t);

  t = huffman_build_tree("books/shakespeare.txt");
  ck_assert_msg(t != NULL, "Tree should not be NULL.");
  ck_assert_int_eq(tree_size(t), 159);
  free(t);
}
END_TEST

START_TEST(test_huffman_find)
{
  TreeNode *t = huffman_build_tree("books/holmes.txt");
  ck_assert_int_eq(tree_size(t), 161);

  EncodeTable *etab = table_build(t);
  ck_assert_msg(etab != NULL, "Encode table should not be NULL.");

  char* e_encode = table_bit_encode(etab, 'e');
  ck_assert_msg(e_encode != NULL, "Problem finding the encoding for 'e'.");

  char c = huffman_find(t, e_encode);
  ck_assert_int_eq(c, 'e');

  char* b_encode = table_bit_encode(etab, 'b');
  c = huffman_find(t, b_encode);
  ck_assert_int_eq(c, 'b');
}
END_TEST

//////////////////////////////////////////////////////////////////////
///////////// table unit tests
//////////////////////////////////////////////////////////////////////

START_TEST(test_table_build)
{
  TreeNode *t = huffman_build_tree("books/holmes.txt");
  ck_assert_int_eq(tree_size(t), 161);

  EncodeTable *etab = table_build(t);
  ck_assert_msg(etab != NULL, "Encode table should not be NULL.");
  free(t);
}
END_TEST

START_TEST(test_table_free)
{
  TreeNode *t = huffman_build_tree("books/holmes.txt");
  ck_assert_int_eq(tree_size(t), 161);

  EncodeTable *etab = table_build(t);
  ck_assert_msg(etab != NULL, "Encode table should not be NULL.");
  free(t);
  free(etab);
}
END_TEST

START_TEST(test_table_encode)
{
  TreeNode *t = huffman_build_tree("books/holmes.txt");
  ck_assert_int_eq(tree_size(t), 161);

  EncodeTable *etab = table_build(t);
  ck_assert_msg(etab != NULL, "Encode table should not be NULL.");

  char* e_encode = table_bit_encode(etab, 'e');
  ck_assert_msg(e_encode != NULL, "Problem finding the encoding for 'e'.");
  ck_assert_int_eq(e_encode[0], '0');
  ck_assert_int_eq(e_encode[1], '0');
  ck_assert_int_eq(e_encode[2], '0');
  ck_assert_int_eq(e_encode[3], '0');
  ck_assert_int_eq(e_encode[4], 0);
  free(e_encode);

  char* space_encode = table_bit_encode(etab, ' ');
  ck_assert_msg(space_encode != NULL, "Problem finding the encoding for ' '.");
  ck_assert_int_eq(space_encode[0], '1');
  ck_assert_int_eq(space_encode[1], '0');
  ck_assert_int_eq(space_encode[2], 0);
  free(space_encode);

  free(t);
  free(etab);
}
END_TEST


START_TEST(test_encode_decode_all_bytes)
{
  Encoder *encoder = encoder_new("books/allbytes.txt", "test/allbytes.he");
  ck_assert_msg(encoder != NULL, "Encoder failed to initialize.");

  int result = encoder_encode(encoder);
  ck_assert_msg(result >= 0, "Problem occurred during encoding.");

  result = encoder_free(encoder);
  ck_assert_msg(result >= 0, "Encoder failed to free properly.");

  Decoder *decoder = decoder_new("test/allbytes.he", "test/allbytes.txt");
  ck_assert_msg(decoder != NULL, "Decoder was null.");

  decoder_decode(decoder);
  decoder_free(decoder);

  FILE *orig = fopen("books/allbytes.txt", "r");
  FILE *copy = fopen("test/allbytes.txt",  "r");
  ck_assert_msg(orig != NULL, "Could not open original.");
  ck_assert_msg(copy != NULL, "Could not open copy.");
  int och = 0;
  int cch = 0;
  while (och >= 0) {
    och = fgetc(orig);
    cch = fgetc(copy);
    ck_assert_msg(och == cch, "Original and copy byte differ.");
  }
  fclose(orig);
  fclose(copy);
  remove("test/allbytes.he");
  remove("test/allbytes.txt");
}
END_TEST

//////////////////////////////////////////////////////////////////////
///////////// Test Suite
//////////////////////////////////////////////////////////////////////

Suite* tester_suite() {
  // Create the test suite:
  Suite* s = suite_create("Public Assignment Tests");

  /**** BEGIN UNIT TESTS ****/
  TCase* tc_inc = tcase_create("Public Tests");
  
  // To use setup/teardown (otherwise skip):
  tcase_add_checked_fixture(tc_inc, setup, teardown);
  tcase_add_test(tc_inc, test_bits_io_open);
  tcase_add_test(tc_inc, test_bits_io_close);
  tcase_add_test(tc_inc, test_bits_io_write_bit);
  tcase_add_test(tc_inc, test_bits_io_read_bit);

  tcase_add_test(tc_inc, test_pqueue_new);
  tcase_add_test(tc_inc, test_pqueue_free);
  tcase_add_test(tc_inc, test_pqueue_enqueue);
  tcase_add_test(tc_inc, test_pqueue_dequeue);

  tcase_add_test(tc_inc, test_tree_new);
  tcase_add_test(tc_inc, test_tree_free);
  tcase_add_test(tc_inc, test_tree_size);

  tcase_add_test(tc_inc, test_huffman_build_tree);
  tcase_add_test(tc_inc, test_huffman_find);

  tcase_add_test(tc_inc, test_table_build);
  tcase_add_test(tc_inc, test_table_free);
  tcase_add_test(tc_inc, test_table_encode);

  tcase_add_test(tc_inc, test_encode_decode_all_bytes);
  // Add unit tests to test suite:
  suite_add_tcase(s, tc_inc);
  /**** END UNIT TESTS   ****/

  // Return the suite:
  return s;
}

//////////////////////////////////////////////////////////////////////
///////////// Test Main Routine
//////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[]) {
  int number_failed;
  Suite* s = tester_suite();
  SRunner* sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
