/********************************************************************

 The tree module implements a binary tree. This is used by the Huffman coding
 algorithm to represent a tree of characters where each character is found in
 a leaf.  We use the path to the character to represent the binary encoding of
 each character.  The goal is to have short paths for the characters that are
 mosts frequently encountered in the input text.

 *******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "tree.h"
#include "pqueue.h"

// This is used to give each node in the tree a unique identifier:
static int ids = 1;

/**
 * Returns a TreeNode object.
 */
TreeNode *tree_new() {
  TreeNode *n = (TreeNode *)(malloc(sizeof(TreeNode)));
  n->id     = ids++;
  n->left   = NULL;
  n->right  = NULL;
  n->next   = NULL;
  n->freq.v = 0;
  n->freq.c = '\0';
  n->type   = INTERNAL;
  return n;
}

/**
 * Returns the size (number of nodes) of the tree.
 */
int tree_size (TreeNode *tree) {
  if (tree == NULL) {
    return 0;
  } else {
    return tree_size(tree->left) + tree_size(tree->right) + 1;
  }
}

/**
 * Returns the total number of characters counted by the tree
 * (the sum of the values) - this is just the frequency in the
 * root node of the tree!
 */
int tree_count (TreeNode *tree) {
  if (tree == NULL) {
    return 0;
  } else {
    return tree->freq.v;
  }
}

/**
 * Deallocates a TreeNode object.
 */
void tree_free (TreeNode *tree) {
  if (tree == NULL) {
    return;
  } else {
    tree_free(tree->left);
    tree_free(tree->right);
    free(tree);
  }
}


/**
 * This is a private function that will print a tree with the given indent.  It
 * recursively visits each node in the tree and prints the information
 * contained in the node at the correct indent.
 */
static void tree_print_indent (TreeNode *tree, int depth, int indent) {
  if (tree != NULL) {
    for (int i = 0; i < indent; i++) {
      printf(tree->type == LEAF ? "-" : " ");
    }

    if (tree->type == LEAF) {
      printf("L/%c/%d/%d\n", tree->freq.c, tree->freq.v, depth);
    } else {
      printf("I/%d/%d\n", tree->freq.v, depth);
    }

    tree_print_indent(tree->left , depth+1, indent+2);
    tree_print_indent(tree->right, depth+1, indent+2);
  }
}


/**
 * Prints the tree.
 */
void tree_print(TreeNode* tree) {
  tree_print_indent(tree, 0, 0);
}


/**
 * This is a private function that recursively serializes the give tree to the
 * provided FILE. The format of the serialized tree node is:
 *
 *   TYPE ID FREQUENCY CHARACTER LEFT_ID RIGHT_ID
 *
 * Where the TYPE is INTERNAL/LEAF, ID is the id given to the tree node when
 * it is created, FREQUENCY is the frequency of the character found in the
 * input text, LEFT_ID is the id of the left child, and RIGHT_ID is the id of
 * the right child.
 *
 * The order of how the tree nodes are serialized is important.  We serialize
 * the child nodes first so when we read the serialized form back in
 * (deserialization) we know that we have already deserialized its children so
 * we can easily assign the correct child TreeNode objects.
 */
static int tree_serialize_rec (TreeNode *tree, FILE *fp) {
  int result;

  // If the tree is a LEAF, then serialize to the correct format and terminate
  // the recursion (base case):
  if (tree->type == LEAF) {
    result = fprintf(fp, "%d %d %d %d %d %d,",
                     tree->type,
                     tree->id,
                     tree->freq.v,
                     tree->freq.c,
                     tree->left  == NULL ? 0 : tree->left ->id,
                     tree->right == NULL ? 0 : tree->right->id);
  } else {
    // If the tree is an INTERNAL node then recursively serialize the left
    // subtree and right subtree.
    if (tree->left != NULL) {
      result = tree_serialize_rec(tree->left, fp);
      if (result < 0) {
        return result;
      }
    }
    if (tree->right != NULL) {
      result = tree_serialize_rec(tree->right, fp);
      if (result < 0) {
        return result;
      }
    }
    // Lastly, serialize the internal node:
    result = fprintf(fp, "%d %d %d %d %d %d,",
                     tree->type,
                     tree->id,
                     tree->freq.v,
                     tree->freq.c,
                     tree->left  == NULL ? 0 : tree->left ->id,
                     tree->right == NULL ? 0 : tree->right->id);
  }
  return result;
}


/**
 * Serializes the given tree to the file fp.  The starting format of the
 * serialized tree is the character '#' and the ending format is also the
 * character '#'.  Returns a negative value if there was an error writing to
 * the file.
 */
int tree_serialize (TreeNode *tree, FILE *fp) {
  int result;
  result = fprintf(fp, "#");
  if (result < 0) {
    return result;
  }
  result = tree_serialize_rec(tree, fp);
  if (result < 0) {
    return result;
  }
  result = fprintf(fp, "#");
  return result;
}


/**
 * Returns a TreeNode object deserialized from the file fp or NULL if an error
 * was encountered in the format.
 */
TreeNode *tree_deserialize (FILE *fp) {
  // First, declare all the important fields we need for a TreeNode:
  int type;
  int id;
  int fval;
  int fch;
  int left;
  int right;
  char delim;

  // Read in the starting delimiter character.  If it is not the
  // correct delimiter (#) then we return NULL.
  if (fscanf(fp, "%c", &delim) != 1 ||
      delim != '#') {
    return NULL;
  }

  // Initialize the delimiter to something other than '#'.
  delim = ',';

  // This is the head of a list of TreeNode objects.
  TreeNode *head = NULL;

  // This is the main loop where we keep reading in serialized records of
  // TreeNodes. We keep looping until we see the ending terminal character
  // '#'.
  while (delim != '#') {
    // Read in a record.
    int count = fscanf(fp, "%d %d %d %d %d %d,",
                       &type, &id, &fval,
                       &fch,  &left, &right);

    // If fscanf reaches the EOF then there is an error in the
    // file format and we return NULL.
    if (count == EOF) {
      return NULL;
    }

    // If fscanf did not read in all of the values then there is
    // an error in the file format, so we return NULL.
    if (count < 6) {
      return NULL;
    }

    // Construct a new TreeNode object from the values that we read in from
    // the serialized file.
    TreeNode *n = tree_new();
    n->id = id;
    n->type = type;
    n->freq.v = fval;
    n->freq.c = fch;

    // Next, we search for the left and right child nodes and add in the new
    // node to the list.  This is a bit tricky and depends on how we serialize
    // the tree.  In particular, the tree_serialize functionality ensures that
    // child nodes are written first so when we read TreeNode's in we are
    // guaranteed that we have their children already in the list.
    //
    // We use the `next` field in the TreeNode structure to keep a simple list
    // of tree nodes.

    TreeNode *c = NULL;  // current node
    TreeNode *p = NULL;  // previous node

    // Iterate through the entire list of TreeNodes.
    for (c = head, p = NULL;
         c != NULL;
         p = c, c = c->next) {
      // If the current node's ID is the left ID of the node we just read in
      // from the serialized format, we then assign the current node to the
      // left child of the new node.
      if (c->id == left) {
        n->left = c;
        // Adjust pointers to remove the child node from the list.
        if (p == NULL) {
          head = head->next;
        } else {
          p->next = c->next;
        }
      }

      // If the current node's ID is the right ID of the node we just read in
      // from the serialized format, we then assign the current node to the
      // right child of the new node.
      if (c->id == right) {
        n->right = c;
        // Adjust pointers to remove the child node from the list.
        if (p == NULL) {
          head = head->next;
        } else {
          p->next = c->next;
        }
      }
    }

    // Add the new node into the list as it might be the child of a TreeNode
    // that we have yet to read in.
    n->next = head;
    head = n;

    // This is the delimiter check.  We grab the next character from the file
    // and check to see if it is the end of the input. The end is marked with
    // a '#' character. If it is not a '#' character we push the character
    // back into the file stream and continue with the loop.
    delim = fgetc(fp);
    if (delim != '#') {
      ungetc(delim, fp);
    }
  }

  if (head == NULL) {
    return NULL;
  }

  // Reset all next pointers to NULL.
  TreeNode *c = NULL;  // current node
  TreeNode *p = NULL;  // previous node
  for (c = head->next, p = NULL;
       c != NULL;
       p = c, c = c->next) {
    if (p != NULL) {
      p->next = NULL;
    }
  }
  p->next = NULL;

  return head;
}
