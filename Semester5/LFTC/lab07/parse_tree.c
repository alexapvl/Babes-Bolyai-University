/*
 * parse_tree.c - Father-Sibling Parse Tree Implementation
 * Lab 7 - LFTC (Formal Languages and Compiler Techniques)
 */

#include "parse_tree.h"

/* Create a new parse tree */
ParseTree *createParseTree() {
  ParseTree *tree = (ParseTree *)malloc(sizeof(ParseTree));
  if (!tree)
    return NULL;

  tree->numNodes = 0;
  return tree;
}

/* Free parse tree memory */
void freeParseTree(ParseTree *tree) {
  if (tree)
    free(tree);
}

/* Add a node to the tree */
int addTreeNode(ParseTree *tree, const char *info, int father) {
  if (tree->numNodes >= MAX_TREE_NODES) {
    fprintf(stderr, "Error: Maximum tree nodes exceeded\n");
    return -1;
  }

  int index = tree->numNodes + 1; /* 1-based indexing */
  TreeNode *node = &tree->nodes[tree->numNodes];

  node->index = index;
  strncpy(node->info, info, MAX_NODE_INFO - 1);
  node->info[MAX_NODE_INFO - 1] = '\0';
  node->father = father;
  node->right_sibling = 0; /* No sibling initially */

  tree->numNodes++;
  return index;
}

/* Set right sibling */
void setRightSibling(ParseTree *tree, int nodeIndex, int siblingIndex) {
  if (nodeIndex <= 0 || nodeIndex > tree->numNodes)
    return;
  tree->nodes[nodeIndex - 1].right_sibling = siblingIndex;
}

/* Print parse tree as father-sibling table */
void printParseTreeTable(ParseTree *tree) {
  printf("\n╔══════════════════════════════════════════════════════════════════"
         "═════╗\n");
  printf("║           PARSE TREE (Father-Sibling Table Representation)         "
         "   ║\n");
  printf("╚════════════════════════════════════════════════════════════════════"
         "═══╝\n\n");

  printf("┌─────────┬────────────────────────────┬────────────┬────────────────"
         "─┐\n");
  printf("│  Index  │         Info               │   Father   │  Right Sibling "
         " │\n");
  printf("├─────────┼────────────────────────────┼────────────┼────────────────"
         "─┤\n");

  for (int i = 0; i < tree->numNodes; i++) {
    TreeNode *node = &tree->nodes[i];
    printf("│  %5d  │  %-24s  │  %8d  │  %13d  │\n", node->index, node->info,
           node->father, node->right_sibling);
  }

  printf("└─────────┴────────────────────────────┴────────────┴────────────────"
         "─┘\n");
  printf("\nTotal nodes: %d\n\n", tree->numNodes);
}

/* Helper: Get first child of a node */
static int getFirstChild(ParseTree *tree, int parentIndex) {
  for (int i = 0; i < tree->numNodes; i++) {
    if (tree->nodes[i].father == parentIndex) {
      return tree->nodes[i].index;
    }
  }
  return 0;
}

/* Helper: Print tree recursively with indentation */
static void printTreeRecursive(ParseTree *tree, int nodeIndex, int depth,
                               bool *lastChild) {
  if (nodeIndex <= 0 || nodeIndex > tree->numNodes)
    return;

  TreeNode *node = &tree->nodes[nodeIndex - 1];

  /* Print indentation */
  for (int i = 0; i < depth - 1; i++) {
    if (lastChild[i]) {
      printf("    ");
    } else {
      printf("│   ");
    }
  }

  if (depth > 0) {
    if (lastChild[depth - 1]) {
      printf("└── ");
    } else {
      printf("├── ");
    }
  }

  printf("%s\n", node->info);

  /* Find and print children */
  int child = getFirstChild(tree, nodeIndex);
  while (child > 0) {
    TreeNode *childNode = &tree->nodes[child - 1];
    bool isLast = (childNode->right_sibling == 0);
    lastChild[depth] = isLast;
    printTreeRecursive(tree, child, depth + 1, lastChild);
    child = childNode->right_sibling;
  }
}

/* Print parse tree visually */
void printParseTreeVisual(ParseTree *tree) {
  printf("\n╔════════════════════════════════════════════════════════════╗\n");
  printf("║                   PARSE TREE (Visual)                       ║\n");
  printf("╚════════════════════════════════════════════════════════════╝\n\n");

  if (tree->numNodes == 0) {
    printf("(empty tree)\n");
    return;
  }

  bool lastChild[100] = {false};
  printTreeRecursive(tree, 1, 0, lastChild); /* Start from root (index 1) */
  printf("\n");
}

/* Get production string (sequence of productions used) */
void getProductionString(ParseTree *tree, char *output, int maxLen) {
  /* This would require tracking production numbers during parsing */
  /* For now, output the tree structure */
  output[0] = '\0';

  int pos = 0;
  for (int i = 0; i < tree->numNodes && pos < maxLen - 50; i++) {
    TreeNode *node = &tree->nodes[i];
    if (node->father > 0) {
      TreeNode *parent = &tree->nodes[node->father - 1];
      pos += snprintf(output + pos, maxLen - pos, "%s->%s ", parent->info,
                      node->info);
    }
  }
}
