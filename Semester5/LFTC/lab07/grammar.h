/*
 * grammar.h - Grammar Data Structures for LL(1) Parser
 * Lab 7 - LFTC (Formal Languages and Compiler Techniques)
 *
 * Defines structures for representing context-free grammars
 */

#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Maximum limits */
#define MAX_SYMBOL_LEN 64
#define MAX_SYMBOLS 100
#define MAX_PRODUCTIONS 200
#define MAX_RHS_LENGTH 20
#define MAX_SET_SIZE 50

/* Special symbols */
#define EPSILON "ε"
#define END_MARKER "$"

/* Symbol types */
typedef enum {
  SYMBOL_TERMINAL,
  SYMBOL_NONTERMINAL,
  SYMBOL_EPSILON,
  SYMBOL_END_MARKER
} SymbolType;

/* Symbol structure */
typedef struct {
  char name[MAX_SYMBOL_LEN];
  SymbolType type;
  int index; /* Index in the symbol table */
} Symbol;

/* Production rule: LHS -> RHS[0] RHS[1] ... RHS[rhsLength-1] */
typedef struct {
  int lhsIndex;            /* Index of LHS non-terminal */
  int rhs[MAX_RHS_LENGTH]; /* Indices of RHS symbols */
  int rhsLength;           /* Number of symbols in RHS */
  int productionNumber;    /* Production number for output */
} Production;

/* Set of symbol indices (for FIRST and FOLLOW sets) */
typedef struct {
  int elements[MAX_SET_SIZE];
  int size;
} SymbolSet;

/* Grammar structure */
typedef struct {
  Symbol symbols[MAX_SYMBOLS]; /* All symbols (terminals + non-terminals) */
  int numSymbols;

  int terminals[MAX_SYMBOLS]; /* Indices of terminal symbols */
  int numTerminals;

  int nonTerminals[MAX_SYMBOLS]; /* Indices of non-terminal symbols */
  int numNonTerminals;

  Production productions[MAX_PRODUCTIONS];
  int numProductions;

  int startSymbol;    /* Index of start symbol */
  int epsilonIndex;   /* Index of epsilon symbol */
  int endMarkerIndex; /* Index of $ symbol */
} Grammar;

/* LL(1) Parsing Table entry */
typedef struct {
  int production; /* Production number to use (-1 = error, -2 = sync) */
} TableEntry;

/* LL(1) Parser state */
typedef struct {
  Grammar *grammar;

  SymbolSet firstSets[MAX_SYMBOLS];  /* FIRST sets for all symbols */
  SymbolSet followSets[MAX_SYMBOLS]; /* FOLLOW sets for non-terminals */

  /* Parsing table: M[nonTerminal][terminal] = production number */
  TableEntry parsingTable[MAX_SYMBOLS][MAX_SYMBOLS];

  bool tableBuilt;
} LL1Parser;

/* Grammar functions */
Grammar *createGrammar();
void freeGrammar(Grammar *g);
int addSymbol(Grammar *g, const char *name, SymbolType type);
int findSymbol(Grammar *g, const char *name);
int addProduction(Grammar *g, int lhs, int *rhs, int rhsLen);
bool loadGrammarFromFile(Grammar *g, const char *filename);
void printGrammar(Grammar *g);

/* Symbol set operations */
void initSet(SymbolSet *set);
bool addToSet(SymbolSet *set, int element);
bool setContains(SymbolSet *set, int element);
bool setUnion(SymbolSet *dest, SymbolSet *src);
bool setUnionExcept(SymbolSet *dest, SymbolSet *src, int exclude);
void printSet(Grammar *g, SymbolSet *set, const char *name);

/* Helper functions */
bool isTerminal(Grammar *g, int symbolIndex);
bool isNonTerminal(Grammar *g, int symbolIndex);
bool isEpsilon(Grammar *g, int symbolIndex);
const char *getSymbolName(Grammar *g, int symbolIndex);

#endif /* GRAMMAR_H */
