/*
 * ll1_parser.h - LL(1) Parser Header
 * Lab 7 - LFTC (Formal Languages and Compiler Techniques)
 */

#ifndef LL1_PARSER_H
#define LL1_PARSER_H

#include "grammar.h"
#include "parse_tree.h"

/* Create and initialize LL(1) parser */
LL1Parser *createLL1Parser(Grammar *grammar);
void freeLL1Parser(LL1Parser *parser);

/* Compute FIRST sets for all symbols */
void computeFirstSets(LL1Parser *parser);

/* Compute FOLLOW sets for all non-terminals */
void computeFollowSets(LL1Parser *parser);

/* Build the LL(1) parsing table */
bool buildParsingTable(LL1Parser *parser);

/* Print FIRST and FOLLOW sets */
void printFirstSets(LL1Parser *parser);
void printFollowSets(LL1Parser *parser);
void printParsingTable(LL1Parser *parser);

/* Compute FIRST set for a sequence of symbols */
void computeFirstOfSequence(LL1Parser *parser, int *symbols, int length,
                            SymbolSet *result);

/*
 * Parse input using the LL(1) algorithm
 *
 * Parameters:
 *   parser     - The LL(1) parser with built parsing table
 *   getToken   - Function that returns next token (terminal symbol name)
 *   tree       - Parse tree to build (father-sibling representation)
 *   verbose    - If true, print parsing steps
 *
 * Returns:
 *   true if parsing succeeded, false otherwise
 */
typedef const char *(*TokenFunction)(void);
bool parseLL1(LL1Parser *parser, TokenFunction getToken, ParseTree *tree,
              bool verbose);

/* Parse from token array (for simple testing) */
bool parseLL1FromTokens(LL1Parser *parser, const char **tokens, int numTokens,
                        ParseTree *tree, bool verbose);

#endif /* LL1_PARSER_H */
