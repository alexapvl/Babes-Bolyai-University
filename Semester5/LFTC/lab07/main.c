/*
 * main.c - LL(1) Parser Driver Program
 * Lab 7 - LFTC (Formal Languages and Compiler Techniques)
 *
 * Usage:
 *   ./ll1_parser --grammar <grammar_file> --input <input_file> [--verbose]
 *   ./ll1_parser --simple --grammar <grammar_file> --input <token_file>
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grammar.h"
#include "lexer.h"
#include "ll1_parser.h"
#include "parse_tree.h"

/* Print usage information */
void printUsage(const char *progName) {
  printf("LL(1) Parser - Lab 7\n");
  printf("====================\n\n");
  printf("Usage:\n");
  printf("  %s --grammar <grammar_file> --input <input_file> [--verbose]\n",
         progName);
  printf("  %s --simple --grammar <grammar_file> --input <token_file>\n\n",
         progName);
  printf("Options:\n");
  printf("  --grammar <file>  Grammar definition file\n");
  printf("  --input <file>    Input file (source code or token sequence)\n");
  printf("  --simple          Use simple tokenizer (space-separated tokens)\n");
  printf("  --verbose         Print detailed parsing steps\n");
  printf("  --help            Show this help message\n\n");
  printf("Examples:\n");
  printf("  %s --grammar grammar_simple.txt --input test_simple.txt --simple\n",
         progName);
  printf("  %s --grammar grammar_minidsl.txt --input ../lab02/program1.txt "
         "--verbose\n",
         progName);
}

/* Parse miniDSL input using lexer */
bool parseMiniDSL(LL1Parser *parser, const char *inputFile, bool verbose) {
  /* Initialize lexer */
  if (!initLexer(inputFile)) {
    return false;
  }

  /* Collect all tokens */
  const char *tokens[1000];
  char tokenStorage[1000][256]; /* Storage for token names */
  int numTokens = 0;

  printf("\n╔════════════════════════════════════════════════════════════╗\n");
  printf("║                    LEXICAL ANALYSIS                         ║\n");
  printf("╚════════════════════════════════════════════════════════════╝\n\n");

  printf("Tokens found:\n");
  printf("─────────────────────────────────────────────────────────────\n");

  Token tok;
  while ((tok = getNextToken()).type != TOKEN_EOF && numTokens < 999) {
    if (tok.type == TOKEN_ERROR) {
      fprintf(stderr, "Lexical error at line %d: '%s'\n", tok.line, tok.value);
      cleanupLexer();
      return false;
    }

    const char *typeName = getTokenTypeName(tok.type);
    strcpy(tokenStorage[numTokens], typeName);
    tokens[numTokens] = tokenStorage[numTokens];

    printf("  %-3d  %-15s  '%s'\n", numTokens + 1, typeName, tok.value);
    numTokens++;
  }

  printf("─────────────────────────────────────────────────────────────\n");
  printf("Total tokens: %d\n", numTokens);

  cleanupLexer();

  /* Create parse tree */
  ParseTree *tree = createParseTree();

  /* Parse */
  bool success = parseLL1FromTokens(parser, tokens, numTokens, tree, verbose);

  if (success) {
    printf("\n✓ Parsing completed successfully!\n");

    /* Print parse tree */
    printParseTreeTable(tree);
    printParseTreeVisual(tree);
  } else {
    printf("\n✗ Parsing failed!\n");
  }

  freeParseTree(tree);
  return success;
}

/* Parse simple token sequence */
bool parseSimple(LL1Parser *parser, const char *inputFile, bool verbose) {
  /* Initialize simple tokenizer */
  if (!initSimpleTokenizer(inputFile)) {
    return false;
  }

  /* Collect all tokens */
  const char *tokens[1000];
  char tokenStorage[1000][64];
  int numTokens = 0;

  printf("\n╔════════════════════════════════════════════════════════════╗\n");
  printf("║                    INPUT TOKENS                             ║\n");
  printf("╚════════════════════════════════════════════════════════════╝\n\n");

  const char *tok;
  while ((tok = getNextSimpleToken()) != NULL && numTokens < 999) {
    strcpy(tokenStorage[numTokens], tok);
    tokens[numTokens] = tokenStorage[numTokens];
    printf("  %d. %s\n", numTokens + 1, tok);
    numTokens++;
  }

  printf("\nTotal tokens: %d\n", numTokens);

  cleanupSimpleTokenizer();

  /* Create parse tree */
  ParseTree *tree = createParseTree();

  /* Parse */
  bool success = parseLL1FromTokens(parser, tokens, numTokens, tree, verbose);

  if (success) {
    printf("\n✓ Parsing completed successfully!\n");

    /* Print parse tree */
    printParseTreeTable(tree);
    printParseTreeVisual(tree);
  } else {
    printf("\n✗ Parsing failed!\n");
  }

  freeParseTree(tree);
  return success;
}

int main(int argc, char **argv) {
  const char *grammarFile = NULL;
  const char *inputFile = NULL;
  bool simpleMode = false;
  bool verbose = false;

  /* Parse command line arguments */
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
      printUsage(argv[0]);
      return 0;
    } else if (strcmp(argv[i], "--grammar") == 0 && i + 1 < argc) {
      grammarFile = argv[++i];
    } else if (strcmp(argv[i], "--input") == 0 && i + 1 < argc) {
      inputFile = argv[++i];
    } else if (strcmp(argv[i], "--simple") == 0) {
      simpleMode = true;
    } else if (strcmp(argv[i], "--verbose") == 0) {
      verbose = true;
    }
  }

  /* Validate arguments */
  if (!grammarFile || !inputFile) {
    fprintf(stderr, "Error: Missing required arguments\n\n");
    printUsage(argv[0]);
    return 1;
  }

  printf("\n");
  printf("╔════════════════════════════════════════════════════════════════════"
         "═══╗\n");
  printf("║              LL(1) PARSER - Lab 7 (LFTC)                           "
         "   ║\n");
  printf("║         Formal Languages and Compiler Techniques                   "
         "   ║\n");
  printf("╚════════════════════════════════════════════════════════════════════"
         "═══╝\n");
  printf("\n");
  printf("Grammar file: %s\n", grammarFile);
  printf("Input file:   %s\n", inputFile);
  printf("Mode:         %s\n",
         simpleMode ? "Simple tokenizer" : "MiniDSL lexer");
  printf("Verbose:      %s\n", verbose ? "Yes" : "No");

  /* Load grammar */
  Grammar *grammar = createGrammar();
  if (!loadGrammarFromFile(grammar, grammarFile)) {
    fprintf(stderr, "Error: Failed to load grammar\n");
    freeGrammar(grammar);
    return 1;
  }

  /* Print grammar */
  printGrammar(grammar);

  /* Create parser */
  LL1Parser *parser = createLL1Parser(grammar);

  /* Build parsing table */
  printf("Building LL(1) parsing table...\n");

  if (!buildParsingTable(parser)) {
    fprintf(stderr, "Warning: Grammar may not be LL(1), conflicts detected\n");
  }

  /* Print FIRST and FOLLOW sets */
  printFirstSets(parser);
  printFollowSets(parser);
  printParsingTable(parser);

  /* Parse input */
  bool success;
  if (simpleMode) {
    success = parseSimple(parser, inputFile, verbose);
  } else {
    success = parseMiniDSL(parser, inputFile, verbose);
  }

  /* Cleanup */
  freeLL1Parser(parser);
  freeGrammar(grammar);

  printf("\n═══════════════════════════════════════════════════════════════════"
         "════\n");

  return success ? 0 : 1;
}
