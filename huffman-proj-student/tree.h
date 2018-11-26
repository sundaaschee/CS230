#ifndef __TREE_H
#define __TREE_H

#include <stdio.h>


/**
 * This structure represents the frequency of a character encountered in the
 * input text. It holds the frequency and the associated character.
 */
typedef struct Frequency Frequency;
struct Frequency {
  // Frequency value:
  int  v;
  // Character:
  char c;
};


/**
 * The NodeType is used to label a tree node as either an INTERNAL node (a
 * node with children) or a LEAF node (a node holding a character).
 */
typedef enum NodeType NodeType;
enum NodeType {
  INTERNAL,
  LEAF
};


/**
 * A TreeNode represents a node in the tree. It has a unique id (id) that
 * identifies the node, the type (INTERNAL/LEAF), the frequency, the left
 * child, and the right child.
 *
 * The `next` field is used to create a linked list of TreeNode objects.  This
 * allows us to use our TreeNode objects in a linked list.  This is used by
 * the deserialization process to maintain a linked list of tree nodes as the
 * original tree is reconstructed.
 */
typedef struct TreeNode TreeNode;
struct TreeNode {
  int        id;
  NodeType   type;
  Frequency  freq;
  TreeNode  *left;
  TreeNode  *right;
  TreeNode  *next;
};


/**
 * Returns a TreeNode object.
 */
TreeNode *tree_new ();


/**
 * Deallocates a TreeNode object.
 */
void tree_free (TreeNode *root);


/**
 * Returns the size of the tree.
 */
int tree_size (TreeNode *root);


/**
 * Returns the total number of characters counted by the tree
 * (the sum of the values), computed recursively
 */
int tree_count (TreeNode *tree);


/**
 * Prints the tree.
 */
void tree_print (TreeNode *root);


/**
 * Serializes the given tree to the file fp.  The starting format of the
 * serialized tree is the character '#' and the ending format is also the
 * character '#'.  Returns a negative value if there was an error writing to
 * the file.
 */
int tree_serialize (TreeNode *root, FILE *fp);

/**
 * Returns a TreeNode object deserialized from the file fp or NULL if an error
 * was encountered in the format.
 */
TreeNode* tree_deserialize (FILE *fp);

#endif
