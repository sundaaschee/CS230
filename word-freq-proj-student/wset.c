#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "wset.h"

/**
 * The WordNode struct is used to represent a node in a linked list of
 * words. It is defined in this file because it is used only within this
 * file. It is used only as part of the implementation of a word set.
 */
typedef struct WordNode WordNode;
struct WordNode {
  int       freq;   // The frequency of the the word.
  char     *word;   // The word itself.
  WordNode *next;   // The next word node in the list.
};

/**
 * A WordSet struct is used to represent a "set". We are implementing a set as
 * a linked list of WordNodes.
 */
struct WordSet {
  int       size;   // The number of elements in the set.
  WordNode *head;   // The starting node in the set.
};

/**
 * The node_free function is used to free a WordNode object.
 */
static void node_free (WordNode *n) {
  if (n != NULL) {
    free(n->word);
    free(n);
  }
}

/**
 * The node_free_all function is used to free a list of WordNode objects.
 */
static void node_free_all (WordNode *n) {
  if (n != NULL) {
    node_free_all(n->next);
    node_free(n);
  }
}

/**
 * copy_word copies a string so that freeing will always work right
 */
static char *copy_word (char *word) {
  assert(word != NULL);
  int len = strlen(word);
  char *copy = (char *)(malloc(len+1));
  assert(copy != NULL);
  strcpy(copy, word);
  return copy;
}

/**
 * The node_new function is used to create a new WordNode object.
 */
static WordNode *node_new (char *word) {
  WordNode *n = (WordNode *)(malloc(sizeof(WordNode)));
  assert(n != NULL);
  n->freq = 0;
  n->word = copy_word(word);
  n->next = NULL;
  return n;
}

/**
 * The wset_new function is used to create a new WordSet object.
 */
WordSet *wset_new () {
  // TODO: Allocate a new WordSet object using `malloc`. Make sure you
  // initialize it properly to avoid possible errors or confusion.
  
  WordSet *wset = (WordSet*)(malloc(sizeof(WordSet)));
  wset->size = 0;
  wset->head = NULL;

  return wset;
}

/**
 * The wset_free function is used to free a WordSet object.
 */
void wset_free (WordSet *wset) {
  // TODO: Deallocate a WordSet object using `free`.
 
 if (wset != NULL)
    node_free_all(wset->head);
  
}

/**
 * Returns the number of distinct words in the set.
 */
int wset_size (WordSet *wset) {
  return wset->size;
}

/**
 * The search function is private to this module and searches a WordSet for a
 * given word.  It return a pointer to the cell that points to the matching
 * WordNode.  This cell may be the `head` field of the WordSet or the `next`
 * field of the preceding WordNode.  If the word is not found then the cell's
 * contents will be NULL (but search never returns NULL -- always the address
 * of one of these cells).  The search function is designed this way to make
 * the rest of the functions easy to write!
 */
static WordNode **search (WordSet *wset, char *word) {
  // TODO: This function will return the address of the cell containing the
  // pointer to the WordNode for the given word, or else the address of the
  // cell containing NULL marking the end of the list.  Note that the cell in
  // question can be either the `head` field of the WordSet struct or the
  // `next` field of some WordNode.
  //
  // Hints:
  // (1) p can start by pointing to the `head` field
  // (2) p will never be NULL, but (*p) might be
  // (3) use strcmp to compare words; it is safe (i.e., you don't need
  //     to use strncmp) because you know you null-terminated all the words
  // (4) When accessing through p, use parentheses thusly (*p) to avoid having
  //     C apply operators in surprising orders
  // (5) What do these expressions mean and are they useful here?
  //     (*p)->word    (*p)->next     &((*p)->next)
  // (6) Draw pictures of the structs and their pointers!
  
  WordNode **p = &(wset->head);
     // put loop here
  while ((*p) != NULL) {
    if(strcmp(word,(*p)->word) == 0){
      return p;
    }
    p = &((*p)->next);
  }
  return p;
}

/**
 * The wset_add function is used to add a new WordNode object to the set if it
 * does not exist. If it does not exist we create a new WordNode and add it to
 * the set, at the end of the list.  Whether or not the word was present
 * before, we increment its frequency, the function returns that frequency as
 * its result.
 */
int wset_add (WordSet *wset, char *word) {
  // TODO: The goal of this function is to add the word `word` to the
  // WordSet. The set implementation is simply a linked list of WordNode's. If
  // a WordNode object does not exist for the given word you must create a new
  // WordNode object and add it to the set.  Whether or not the node is new,
  // update its frequency, and return that frequency as the result of the
  // function.
  //
  // The search function is designed to make this function relatively easy to
  // write!
  //
  // This function is used by the wordfreq.c program.

 WordNode **p = search(wset,word);
  if ((*p) != NULL){
    ((*p)->freq)++;
     return ((*p)->freq);
  }
  else {
    WordNode *new_node = node_new(word);
    WordNode **ptr = &(wset->head);
    new_node->freq = 1;
    wset->size++;

    while((*ptr) != NULL){
      ptr = &((*ptr)->next);
    }

    (*ptr) = new_node;
    return (new_node->freq);
  }
}

/**
 * The wset_fast_add function adds a new word to the set if it does not exist,
 * adding it at the beginning of the list. In addition if the word does exist
 * in the set it will move the WordNode object to the front of the linked
 * list. This will improve performance of adding words to the set and
 * incrementing their frequency because the most frequently accessed nodes
 * will move near the front of the list. Thus, this will result in shorter
 * search times for typical inputs (it would not help in the case where each
 * word occurs once).  The function returns the frequency of the word.
 */
int wset_fast_add (WordSet *wset, char *word) {
  // TODO: Implement a fast add function. This function will also add a word
  // to the set if it is not already present, updating the word's frequency in
  // any case. In addition, if the word already exists in the set you must
  // move the associated WordNode object to the front of the list. This will
  // result in frequently seen words residing at the beginning of the list -
  // which will reduce the amount of time it takes to find them.  Likewise,
  // new words should also be added at the head of the list.  In sum, the
  // node, whether new or already present, is inserted at the beginning of the
  // list and its frequency is updated.  If the node was already present, the
  // preceding link must be set to bypass the node, unlinking it from the
  // list, before moving the node to the head of the list.
  //
  // The search function is designed to make this function relatively easy to
  // write!
  //
  // This function is used by the wordfreq-fast.c program.

  WordNode **p = search(wset, word);
  WordNode *node;

  if((*p) == NULL){
    node = node_new(word);
    wset->size++;
  }
  else{
    node = *p;
    *p = node->next;
  }

  node->freq++;
  node->next = wset->head;
  wset->head = node;
  
  return wset->head->freq;
}

/**
 * The wset_remove function is used to remove the given word from a WordSet
 * object. It returns the frequency of the word it removed or -1 if the word
 * was not found.
 */
int wset_remove (WordSet *wset, char *word) {
  // TODO: This function must remove the word from the set.  If the the word
  // is not in the set, it should return -1.  If the word is in the set, it
  // should return the word's frequency.  Don't forget to free the word's node
  // and string, and to adjust the size of the word set.  Note that the search
  // function is designed to make this function easy to write!

 WordNode** p = search(wset,word);
  if (*p != NULL){
      int freq = (*p)->freq;
      WordNode *to_remove = *p;
      WordNode *next = (*p)->next;
      node_free(to_remove);
      *p = next;
      wset->size--;
      return freq;
  } 
  return -1;   
}

/**
 * The wset_exists function is used to test the existence of the given word in
 * a WordSet object. It returns 1 if the word exists in the set; 0 otherwise.
 */
int wset_exists (WordSet *wset, char *word) {
  // TODO: search the linked list for the word. If you do not find the word
  // then you return 0, if you do find the word, return 1

   WordNode **p = search(wset,word);
  if((*p) == NULL){
      return 0;
  } 
  return 1;
}

/**
 * The wset_freq function returns the frequency of the given word in the
 * WordSet object. If the word does not exist it returns 0.
 */
int wset_freq (WordSet *wset, char *word) {
  // TODO: search the for the word - return its frequency.  If you do not find
  // the word then return 0.

   WordNode** p = search(wset,word);
  if ((*p) != NULL){
    int freq = (*p)->freq;
      return freq;
  } 
  return 0;
}

/**
 * The wset_print function prints the words and frequencies in the WordSet
 * object.
 */
void wset_print (WordSet *wset) {
  int longest = 0;
  for (WordNode *p = wset->head; p != NULL; p = p->next) {
    int len = strlen(p->word);
    if (longest < len)
      longest = len;
  }
  for (WordNode *p = wset->head; p != NULL; p = p->next) {
    printf("%-*s : %d\n", longest, p->word, p->freq);
  }
}
