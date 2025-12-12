/*
 * parse_tree.h - Father-Sibling Parse Tree Representation
 * Lab 7 - LFTC (Formal Languages and Compiler Techniques)
 */

#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_NODES 1000
#define MAX_NODE_INFO 64

/*
 * Parse Tree Node using Father-Sibling representation
 *
 * This representation uses two pointers per node:
 *   - father: index of parent node (0 for root)
 *   - right_sibling: index of next sibling (0 if none)
 *
 * This is memory efficient and allows easy tree traversal.
 */
typedef struct {
  int index;                /* Node index (1-based) */
  char info[MAX_NODE_INFO]; /* Symbol name or terminal value */
  int father;               /* Parent node index (0 = no parent/root) */
  int right_sibling;        /* Right sibling index (0 = no sibling) */
} TreeNode;

/* Parse Tree structure */
typedef struct {
  TreeNode nodes[MAX_TREE_NODES];
  int numNodes;
} ParseTree;

/* Create and initialize parse tree */
ParseTree *createParseTree();
void freeParseTree(ParseTree *tree);

/* Add a node to the tree, returns node index */
int addTreeNode(ParseTree *tree, const char *info, int father);

/* Set right sibling relationship */
void setRightSibling(ParseTree *tree, int nodeIndex, int siblingIndex);

/* Print the parse tree as a table */
void printParseTreeTable(ParseTree *tree);

/* Print the parse tree in a visual tree format */
void printParseTreeVisual(ParseTree *tree);

/* Get production string from parse tree */
void getProductionString(ParseTree *tree, char *output, int maxLen);

#endif /* PARSE_TREE_H */
