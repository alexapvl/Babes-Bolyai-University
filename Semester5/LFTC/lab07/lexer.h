/*
 * lexer.h - Lexer Interface for LL(1) Parser
 * Lab 7 - LFTC (Formal Languages and Compiler Techniques)
 */

#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>
#include <stdio.h>

/* Token types matching the grammar terminals */
typedef enum {
  TOKEN_EOF = 0,

  /* Keywords */
  TOKEN_VAR,
  TOKEN_LOAD,
  TOKEN_FILTER,
  TOKEN_MAP,
  TOKEN_SELECT,
  TOKEN_SAVE,
  TOKEN_FOR,
  TOKEN_WHILE,
  TOKEN_IN,
  TOKEN_DO,
  TOKEN_END,
  TOKEN_TO,

  /* Operators */
  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_MULTIPLY,
  TOKEN_DIVIDE,
  TOKEN_ASSIGN,
  TOKEN_EQ,
  TOKEN_NEQ,
  TOKEN_GT,
  TOKEN_LT,
  TOKEN_GTE,
  TOKEN_LTE,

  /* Delimiters */
  TOKEN_SEMICOLON,
  TOKEN_COMMA,
  TOKEN_LPAREN,
  TOKEN_RPAREN,

  /* Literals and identifiers */
  TOKEN_IDENTIFIER,
  TOKEN_NUMBER,
  TOKEN_STRING,

  /* Error */
  TOKEN_ERROR
} TokenType;

/* Token structure */
typedef struct {
  TokenType type;
  char value[256]; /* The actual token text */
  int line;        /* Line number */
} Token;

/* Initialize lexer with input file */
bool initLexer(const char *filename);

/* Clean up lexer resources */
void cleanupLexer();

/* Get next token */
Token getNextToken();

/* Get token type name as string (for grammar matching) */
const char *getTokenTypeName(TokenType type);

/* Simple tokenizer for test sequences (space-separated) */
bool initSimpleTokenizer(const char *filename);
const char *getNextSimpleToken();
void cleanupSimpleTokenizer();

#endif /* LEXER_H */
