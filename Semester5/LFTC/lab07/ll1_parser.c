/*
 * ll1_parser.c - LL(1) Parser Implementation
 * Lab 7 - LFTC (Formal Languages and Compiler Techniques)
 *
 * Implements:
 *   - FIRST set computation
 *   - FOLLOW set computation
 *   - LL(1) parsing table construction
 *   - Predictive parsing algorithm
 */

#include "ll1_parser.h"
#include <string.h>

/* Simple stack for parsing */
#define MAX_STACK_SIZE 500

typedef struct {
  int items[MAX_STACK_SIZE];
  int treeNodes[MAX_STACK_SIZE]; /* Corresponding tree node indices */
  int top;
} ParseStack;

static void stackInit(ParseStack *s) { s->top = -1; }
static bool stackEmpty(ParseStack *s) { return s->top == -1; }
static bool stackPush(ParseStack *s, int item, int treeNode) {
  if (s->top >= MAX_STACK_SIZE - 1)
    return false;
  s->top++;
  s->items[s->top] = item;
  s->treeNodes[s->top] = treeNode;
  return true;
}
static int stackPop(ParseStack *s, int *treeNode) {
  if (s->top < 0)
    return -1;
  if (treeNode)
    *treeNode = s->treeNodes[s->top];
  return s->items[s->top--];
}
static int stackTop(ParseStack *s, int *treeNode) {
  if (s->top < 0)
    return -1;
  if (treeNode)
    *treeNode = s->treeNodes[s->top];
  return s->items[s->top];
}

/* Create LL(1) parser */
LL1Parser *createLL1Parser(Grammar *grammar) {
  LL1Parser *parser = (LL1Parser *)malloc(sizeof(LL1Parser));
  if (!parser)
    return NULL;

  parser->grammar = grammar;
  parser->tableBuilt = false;

  /* Initialize all sets */
  for (int i = 0; i < MAX_SYMBOLS; i++) {
    initSet(&parser->firstSets[i]);
    initSet(&parser->followSets[i]);
  }

  /* Initialize parsing table to error (-1) */
  for (int i = 0; i < MAX_SYMBOLS; i++) {
    for (int j = 0; j < MAX_SYMBOLS; j++) {
      parser->parsingTable[i][j].production = -1;
    }
  }

  return parser;
}

/* Free parser */
void freeLL1Parser(LL1Parser *parser) {
  if (parser)
    free(parser);
}

/*
 * Compute FIRST sets for all symbols
 *
 * Algorithm:
 * 1. For terminal a: FIRST(a) = {a}
 * 2. For epsilon: FIRST(ε) = {ε}
 * 3. For non-terminal A with production A → X1 X2 ... Xn:
 *    - Add FIRST(X1) \ {ε} to FIRST(A)
 *    - If ε ∈ FIRST(X1), add FIRST(X2) \ {ε} to FIRST(A)
 *    - Continue until Xi where ε ∉ FIRST(Xi)
 *    - If ε ∈ FIRST(Xi) for all i, add ε to FIRST(A)
 * 4. Repeat step 3 until no changes
 */
void computeFirstSets(LL1Parser *parser) {
  Grammar *g = parser->grammar;

  /* Initialize FIRST sets for terminals and epsilon */
  for (int i = 0; i < g->numSymbols; i++) {
    if (isTerminal(g, i) || isEpsilon(g, i)) {
      addToSet(&parser->firstSets[i], i);
    }
  }

  /* Also add end marker */
  addToSet(&parser->firstSets[g->endMarkerIndex], g->endMarkerIndex);

  /* Iterate until no changes */
  bool changed = true;
  while (changed) {
    changed = false;

    /* For each production A → α */
    for (int p = 0; p < g->numProductions; p++) {
      Production *prod = &g->productions[p];
      int A = prod->lhsIndex;

      /* Compute FIRST(α) and add to FIRST(A) */
      bool allHaveEpsilon = true;

      for (int i = 0; i < prod->rhsLength && allHaveEpsilon; i++) {
        int Xi = prod->rhs[i];

        /* Add FIRST(Xi) \ {ε} to FIRST(A) */
        if (setUnionExcept(&parser->firstSets[A], &parser->firstSets[Xi],
                           g->epsilonIndex)) {
          changed = true;
        }

        /* Check if ε ∈ FIRST(Xi) */
        if (!setContains(&parser->firstSets[Xi], g->epsilonIndex)) {
          allHaveEpsilon = false;
        }
      }

      /* If all symbols can derive ε, add ε to FIRST(A) */
      if (allHaveEpsilon) {
        if (addToSet(&parser->firstSets[A], g->epsilonIndex)) {
          changed = true;
        }
      }
    }
  }
}

/*
 * Compute FOLLOW sets for all non-terminals
 *
 * Algorithm:
 * 1. Add $ to FOLLOW(S) where S is start symbol
 * 2. For each production A → αBβ:
 *    - Add FIRST(β) \ {ε} to FOLLOW(B)
 * 3. For each production A → αB or A → αBβ where ε ∈ FIRST(β):
 *    - Add FOLLOW(A) to FOLLOW(B)
 * 4. Repeat steps 2-3 until no changes
 */
void computeFollowSets(LL1Parser *parser) {
  Grammar *g = parser->grammar;

  /* Add $ to FOLLOW(S) */
  addToSet(&parser->followSets[g->startSymbol], g->endMarkerIndex);

  /* Iterate until no changes */
  bool changed = true;
  while (changed) {
    changed = false;

    /* For each production A → X1 X2 ... Xn */
    for (int p = 0; p < g->numProductions; p++) {
      Production *prod = &g->productions[p];
      int A = prod->lhsIndex;

      for (int i = 0; i < prod->rhsLength; i++) {
        int B = prod->rhs[i];

        /* Only process non-terminals */
        if (!isNonTerminal(g, B))
          continue;

        /* β is the rest of the production after B */
        /* Compute FIRST(β) */
        bool betaHasEpsilon = true;

        for (int j = i + 1; j < prod->rhsLength && betaHasEpsilon; j++) {
          int Xj = prod->rhs[j];

          /* Add FIRST(Xj) \ {ε} to FOLLOW(B) */
          if (setUnionExcept(&parser->followSets[B], &parser->firstSets[Xj],
                             g->epsilonIndex)) {
            changed = true;
          }

          /* Check if ε ∈ FIRST(Xj) */
          if (!setContains(&parser->firstSets[Xj], g->epsilonIndex)) {
            betaHasEpsilon = false;
          }
        }

        /* If B is at the end or β can derive ε, add FOLLOW(A) to FOLLOW(B) */
        if (betaHasEpsilon) {
          if (setUnion(&parser->followSets[B], &parser->followSets[A])) {
            changed = true;
          }
        }
      }
    }
  }
}

/* Compute FIRST set for a sequence of symbols */
void computeFirstOfSequence(LL1Parser *parser, int *symbols, int length,
                            SymbolSet *result) {
  Grammar *g = parser->grammar;
  initSet(result);

  bool allHaveEpsilon = true;

  for (int i = 0; i < length && allHaveEpsilon; i++) {
    int Xi = symbols[i];

    /* Add FIRST(Xi) \ {ε} to result */
    setUnionExcept(result, &parser->firstSets[Xi], g->epsilonIndex);

    /* Check if ε ∈ FIRST(Xi) */
    if (!setContains(&parser->firstSets[Xi], g->epsilonIndex)) {
      allHaveEpsilon = false;
    }
  }

  /* If all symbols can derive ε, add ε */
  if (allHaveEpsilon) {
    addToSet(result, g->epsilonIndex);
  }
}

/*
 * Build the LL(1) parsing table
 *
 * Algorithm:
 * For each production A → α:
 *   1. For each terminal a in FIRST(α):
 *      - Add A → α to M[A, a]
 *   2. If ε ∈ FIRST(α):
 *      - For each terminal b in FOLLOW(A):
 *        - Add A → α to M[A, b]
 *
 * Returns false if there are conflicts (grammar is not LL(1))
 */
bool buildParsingTable(LL1Parser *parser) {
  Grammar *g = parser->grammar;
  bool hasConflicts = false;

  /* First compute FIRST and FOLLOW sets */
  computeFirstSets(parser);
  computeFollowSets(parser);

  /* For each production */
  for (int p = 0; p < g->numProductions; p++) {
    Production *prod = &g->productions[p];
    int A = prod->lhsIndex;

    /* Compute FIRST(α) where α is the RHS */
    SymbolSet firstAlpha;
    computeFirstOfSequence(parser, prod->rhs, prod->rhsLength, &firstAlpha);

    /* For each terminal a in FIRST(α) */
    for (int i = 0; i < firstAlpha.size; i++) {
      int a = firstAlpha.elements[i];

      if (a == g->epsilonIndex)
        continue; /* Skip epsilon */

      if (parser->parsingTable[A][a].production != -1 &&
          parser->parsingTable[A][a].production != p) {
        fprintf(stderr, "LL(1) Conflict at M[%s, %s]: productions %d and %d\n",
                g->symbols[A].name, g->symbols[a].name,
                parser->parsingTable[A][a].production + 1, p + 1);
        hasConflicts = true;
      }
      parser->parsingTable[A][a].production = p;
    }

    /* If ε ∈ FIRST(α) */
    if (setContains(&firstAlpha, g->epsilonIndex)) {
      /* For each terminal b in FOLLOW(A) */
      for (int i = 0; i < parser->followSets[A].size; i++) {
        int b = parser->followSets[A].elements[i];

        if (parser->parsingTable[A][b].production != -1 &&
            parser->parsingTable[A][b].production != p) {
          fprintf(stderr,
                  "LL(1) Conflict at M[%s, %s]: productions %d and %d\n",
                  g->symbols[A].name, g->symbols[b].name,
                  parser->parsingTable[A][b].production + 1, p + 1);
          hasConflicts = true;
        }
        parser->parsingTable[A][b].production = p;
      }
    }
  }

  parser->tableBuilt = !hasConflicts;
  return !hasConflicts;
}

/* Print FIRST sets */
void printFirstSets(LL1Parser *parser) {
  Grammar *g = parser->grammar;

  printf("\n╔════════════════════════════════════════════════════════════╗\n");
  printf("║                       FIRST SETS                           ║\n");
  printf("╚════════════════════════════════════════════════════════════╝\n\n");

  for (int i = 0; i < g->numNonTerminals; i++) {
    int nt = g->nonTerminals[i];
    char name[80];
    snprintf(name, sizeof(name), "FIRST(%s)", g->symbols[nt].name);
    printSet(g, &parser->firstSets[nt], name);
  }
  printf("\n");
}

/* Print FOLLOW sets */
void printFollowSets(LL1Parser *parser) {
  Grammar *g = parser->grammar;

  printf("\n╔════════════════════════════════════════════════════════════╗\n");
  printf("║                      FOLLOW SETS                           ║\n");
  printf("╚════════════════════════════════════════════════════════════╝\n\n");

  for (int i = 0; i < g->numNonTerminals; i++) {
    int nt = g->nonTerminals[i];
    char name[80];
    snprintf(name, sizeof(name), "FOLLOW(%s)", g->symbols[nt].name);
    printSet(g, &parser->followSets[nt], name);
  }
  printf("\n");
}

/* Print parsing table */
void printParsingTable(LL1Parser *parser) {
  Grammar *g = parser->grammar;

  printf("\n╔════════════════════════════════════════════════════════════╗\n");
  printf("║                   LL(1) PARSING TABLE                       ║\n");
  printf("╚════════════════════════════════════════════════════════════╝\n\n");

  /* Print header */
  printf("%-20s", "");
  for (int t = 0; t < g->numTerminals; t++) {
    printf("%-10s", g->symbols[g->terminals[t]].name);
  }
  printf("%-10s\n", "$");

  printf("─────────────────────────────────────────────────────────────────\n");

  /* Print rows */
  for (int nt = 0; nt < g->numNonTerminals; nt++) {
    int ntIndex = g->nonTerminals[nt];
    printf("%-20s", g->symbols[ntIndex].name);

    for (int t = 0; t < g->numTerminals; t++) {
      int tIndex = g->terminals[t];
      int prod = parser->parsingTable[ntIndex][tIndex].production;
      if (prod >= 0) {
        printf("%-10d", prod + 1); /* 1-based production number */
      } else {
        printf("%-10s", "");
      }
    }

    /* End marker column */
    int prod = parser->parsingTable[ntIndex][g->endMarkerIndex].production;
    if (prod >= 0) {
      printf("%-10d", prod + 1);
    } else {
      printf("%-10s", "");
    }
    printf("\n");
  }
  printf("\n");
}

/*
 * LL(1) Predictive Parsing Algorithm
 *
 * Uses a stack and parsing table to parse input.
 * Builds the parse tree as father-sibling table.
 */
bool parseLL1FromTokens(LL1Parser *parser, const char **tokens, int numTokens,
                        ParseTree *tree, bool verbose) {
  Grammar *g = parser->grammar;
  ParseStack stack;
  stackInit(&stack);

  int tokenIndex = 0;

  /* Initialize tree with root */
  int rootNode = addTreeNode(tree, g->symbols[g->startSymbol].name, 0);

  /* Push $ and start symbol onto stack */
  stackPush(&stack, g->endMarkerIndex, 0);
  stackPush(&stack, g->startSymbol, rootNode);

  if (verbose) {
    printf(
        "\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                    PARSING STEPS                            ║\n");
    printf(
        "╚════════════════════════════════════════════════════════════╝\n\n");
    printf("%-5s %-25s %-15s %-20s\n", "Step", "Stack", "Input", "Action");
    printf(
        "─────────────────────────────────────────────────────────────────\n");
  }

  int step = 1;

  while (!stackEmpty(&stack)) {
    int treeNode;
    int X = stackTop(&stack, &treeNode);

    /* Get current input token */
    const char *currentToken =
        (tokenIndex < numTokens) ? tokens[tokenIndex] : END_MARKER;
    int a = findSymbol(g, currentToken);

    if (a < 0) {
      /* Token not in grammar - check if it's an identifier/number placeholder
       */
      /* Try common terminal names */
      if (findSymbol(g, "IDENTIFIER") >= 0) {
        /* This might be an identifier value */
        a = findSymbol(g, "IDENTIFIER");
      } else if (findSymbol(g, "NUMBER") >= 0) {
        a = findSymbol(g, "NUMBER");
      } else {
        fprintf(stderr, "Error: Unknown token '%s'\n", currentToken);
        return false;
      }
    }

    if (verbose) {
      printf("%-5d %-25s %-15s ", step++, g->symbols[X].name, currentToken);
    }

    if (X == g->endMarkerIndex) {
      if (a == g->endMarkerIndex) {
        if (verbose)
          printf("Accept!\n");
        stackPop(&stack, NULL);
      } else {
        fprintf(stderr, "Parse error: Expected end of input\n");
        return false;
      }
    } else if (isTerminal(g, X) || isEpsilon(g, X)) {
      if (isEpsilon(g, X)) {
        /* Epsilon - just pop from stack */
        if (verbose)
          printf("Pop ε\n");
        stackPop(&stack, NULL);
      } else if (X == a) {
        /* Match - pop and advance input */
        if (verbose)
          printf("Match '%s'\n", currentToken);
        stackPop(&stack, NULL);
        tokenIndex++;
      } else {
        fprintf(stderr, "Parse error: Expected '%s', got '%s'\n",
                g->symbols[X].name, currentToken);
        return false;
      }
    } else if (isNonTerminal(g, X)) {
      /* Look up parsing table */
      int prodIndex = parser->parsingTable[X][a].production;

      if (prodIndex < 0) {
        fprintf(stderr, "Parse error: No entry in table for M[%s, %s]\n",
                g->symbols[X].name, g->symbols[a].name);
        return false;
      }

      Production *prod = &g->productions[prodIndex];

      if (verbose) {
        printf("Apply %d: %s -> ", prod->productionNumber, g->symbols[X].name);
        for (int i = 0; i < prod->rhsLength; i++) {
          printf("%s ", g->symbols[prod->rhs[i]].name);
        }
        printf("\n");
      }

      /* Pop X from stack */
      stackPop(&stack, NULL);

      /* Push RHS symbols in reverse order */
      /* Also create tree nodes for children */
      int prevSibling = 0;
      int childNodes[MAX_RHS_LENGTH];

      for (int i = 0; i < prod->rhsLength; i++) {
        childNodes[i] =
            addTreeNode(tree, g->symbols[prod->rhs[i]].name, treeNode);
      }

      /* Link siblings (left to right) */
      for (int i = 0; i < prod->rhsLength - 1; i++) {
        setRightSibling(tree, childNodes[i], childNodes[i + 1]);
      }

      /* Push in reverse order (so first symbol is on top) */
      for (int i = prod->rhsLength - 1; i >= 0; i--) {
        stackPush(&stack, prod->rhs[i], childNodes[i]);
      }
    }
  }

  if (verbose) {
    printf("─────────────────────────────────────────────────────────────────\n"
           "\n");
  }

  return tokenIndex >= numTokens;
}

/* Parse using a token function */
bool parseLL1(LL1Parser *parser, TokenFunction getToken, ParseTree *tree,
              bool verbose) {
  /* Collect all tokens first */
  const char *tokens[1000];
  int numTokens = 0;

  const char *token;
  while ((token = getToken()) != NULL && numTokens < 1000) {
    tokens[numTokens++] = token;
  }

  return parseLL1FromTokens(parser, tokens, numTokens, tree, verbose);
}
