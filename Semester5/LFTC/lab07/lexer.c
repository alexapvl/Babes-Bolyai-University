/*
 * lexer.c - Lexer Implementation for LL(1) Parser
 * Lab 7 - LFTC (Formal Languages and Compiler Techniques)
 *
 * Adapted from lab06 lexer with FA-based validation.
 * Modified to work standalone without Bison.
 */

#include "lexer.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/* Lexer state */
static FILE *inputFile = NULL;
static int currentLine = 1;

/* Character class types for FA transitions */
typedef enum {
  CHAR_CLASS_LITERAL,
  CHAR_CLASS_LETTER,
  CHAR_CLASS_DIGIT,
  CHAR_CLASS_NONZERO,
  CHAR_CLASS_UNDERSCORE,
  CHAR_CLASS_STRING_CHAR,
  CHAR_CLASS_ANY
} CharClass;

/* FA Transition structure */
typedef struct {
  int from_state;
  CharClass char_class;
  char literal;
  int to_state;
} Transition;

/* Finite Automaton structure */
typedef struct {
  int num_states;
  int num_transitions;
  Transition *transitions;
  int start_state;
  int num_final_states;
  int *final_states;
} FiniteAutomata;

/* FA instances */
static FiniteAutomata identifier_fa;
static FiniteAutomata number_fa;
static FiniteAutomata string_fa;
static bool fasInitialized = false;

/* Keyword list */
static const char *keywords[] = {"VAR",  "LOAD", "FILTER", "MAP", "SELECT",
                                 "SAVE", "FOR",  "WHILE",  "IN",  "DO",
                                 "END",  "TO",   NULL};

/* Create identifier FA: [a-zA-Z][a-zA-Z0-9_]* */
static Transition id_transitions[] = {{0, CHAR_CLASS_LETTER, '\0', 1},
                                      {1, CHAR_CLASS_LETTER, '\0', 1},
                                      {1, CHAR_CLASS_DIGIT, '\0', 1},
                                      {1, CHAR_CLASS_UNDERSCORE, '\0', 1}};
static int id_final_states[] = {1};

/* Create number FA: 0 | [1-9][0-9]* */
static Transition num_transitions[] = {{0, CHAR_CLASS_LITERAL, '0', 1},
                                       {0, CHAR_CLASS_NONZERO, '\0', 2},
                                       {2, CHAR_CLASS_DIGIT, '\0', 2}};
static int num_final_states[] = {1, 2};

/* Create string FA: "[a-zA-Z0-9_ ./]*" */
static Transition str_transitions[] = {{0, CHAR_CLASS_LITERAL, '"', 1},
                                       {1, CHAR_CLASS_STRING_CHAR, '\0', 1},
                                       {1, CHAR_CLASS_LITERAL, '"', 2}};
static int str_final_states[] = {2};

/* Initialize FAs */
static void initFAs() {
  if (fasInitialized)
    return;

  identifier_fa.num_states = 2;
  identifier_fa.num_transitions = 4;
  identifier_fa.transitions = id_transitions;
  identifier_fa.start_state = 0;
  identifier_fa.num_final_states = 1;
  identifier_fa.final_states = id_final_states;

  number_fa.num_states = 3;
  number_fa.num_transitions = 3;
  number_fa.transitions = num_transitions;
  number_fa.start_state = 0;
  number_fa.num_final_states = 2;
  number_fa.final_states = num_final_states;

  string_fa.num_states = 3;
  string_fa.num_transitions = 3;
  string_fa.transitions = str_transitions;
  string_fa.start_state = 0;
  string_fa.num_final_states = 1;
  string_fa.final_states = str_final_states;

  fasInitialized = true;
}

/* Check if character matches character class */
static bool matchesCharClass(char c, CharClass char_class, char literal) {
  switch (char_class) {
  case CHAR_CLASS_LITERAL:
    return c == literal;
  case CHAR_CLASS_LETTER:
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
  case CHAR_CLASS_DIGIT:
    return c >= '0' && c <= '9';
  case CHAR_CLASS_NONZERO:
    return c >= '1' && c <= '9';
  case CHAR_CLASS_UNDERSCORE:
    return c == '_';
  case CHAR_CLASS_STRING_CHAR:
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') || c == '_' || c == ' ' || c == '.' ||
           c == '/';
  case CHAR_CLASS_ANY:
    return true;
  default:
    return false;
  }
}

/* Validate token with FA */
static bool validateWithFA(FiniteAutomata *fa, const char *input) {
  int current_state = fa->start_state;
  int len = strlen(input);

  for (int i = 0; i < len; i++) {
    char c = input[i];
    bool found = false;

    for (int j = 0; j < fa->num_transitions; j++) {
      Transition *t = &fa->transitions[j];
      if (t->from_state == current_state &&
          matchesCharClass(c, t->char_class, t->literal)) {
        current_state = t->to_state;
        found = true;
        break;
      }
    }

    if (!found)
      return false;
  }

  for (int i = 0; i < fa->num_final_states; i++) {
    if (current_state == fa->final_states[i])
      return true;
  }
  return false;
}

/* Check if token is a keyword */
static int isKeyword(const char *token) {
  for (int i = 0; keywords[i] != NULL; i++) {
    if (strcmp(token, keywords[i]) == 0)
      return i;
  }
  return -1;
}

/* Check if character is operator or delimiter */
static bool isOpOrDelim(char c) {
  return (c == '=' || c == '!' || c == '>' || c == '<' || c == '+' ||
          c == '-' || c == '*' || c == '/' || c == ';' || c == ',' ||
          c == '(' || c == ')');
}

/* Initialize lexer */
bool initLexer(const char *filename) {
  inputFile = fopen(filename, "r");
  if (!inputFile) {
    fprintf(stderr, "Error: Cannot open file '%s'\n", filename);
    return false;
  }
  currentLine = 1;
  initFAs();
  return true;
}

/* Cleanup lexer */
void cleanupLexer() {
  if (inputFile) {
    fclose(inputFile);
    inputFile = NULL;
  }
}

/* Get next token */
Token getNextToken() {
  Token token;
  token.type = TOKEN_EOF;
  token.value[0] = '\0';
  token.line = currentLine;

  if (!inputFile)
    return token;

  char c;

  /* Skip whitespace */
  while ((c = fgetc(inputFile)) != EOF) {
    if (c == '\n') {
      currentLine++;
      continue;
    }
    if (c == ' ' || c == '\t' || c == '\r')
      continue;
    break;
  }

  if (c == EOF)
    return token;

  token.line = currentLine;

  /* Handle strings */
  if (c == '"') {
    int pos = 0;
    token.value[pos++] = c;
    while ((c = fgetc(inputFile)) != EOF && c != '"' && pos < 255) {
      token.value[pos++] = c;
    }
    if (c == '"')
      token.value[pos++] = c;
    token.value[pos] = '\0';

    if (validateWithFA(&string_fa, token.value)) {
      token.type = TOKEN_STRING;
    } else {
      token.type = TOKEN_ERROR;
    }
    return token;
  }

  /* Handle two-character operators */
  if (c == '=' || c == '!' || c == '>' || c == '<') {
    token.value[0] = c;
    char next = fgetc(inputFile);
    if (next == '=') {
      token.value[1] = '=';
      token.value[2] = '\0';
      if (c == '=')
        token.type = TOKEN_EQ;
      else if (c == '!')
        token.type = TOKEN_NEQ;
      else if (c == '>')
        token.type = TOKEN_GTE;
      else if (c == '<')
        token.type = TOKEN_LTE;
    } else {
      token.value[1] = '\0';
      ungetc(next, inputFile);
      if (c == '=')
        token.type = TOKEN_ASSIGN;
      else if (c == '>')
        token.type = TOKEN_GT;
      else if (c == '<')
        token.type = TOKEN_LT;
      else
        token.type = TOKEN_ERROR;
    }
    return token;
  }

  /* Handle single-character operators and delimiters */
  if (c == '+') {
    token.value[0] = c;
    token.value[1] = '\0';
    token.type = TOKEN_PLUS;
    return token;
  }
  if (c == '-') {
    token.value[0] = c;
    token.value[1] = '\0';
    token.type = TOKEN_MINUS;
    return token;
  }
  if (c == '*') {
    token.value[0] = c;
    token.value[1] = '\0';
    token.type = TOKEN_MULTIPLY;
    return token;
  }
  if (c == '/') {
    token.value[0] = c;
    token.value[1] = '\0';
    token.type = TOKEN_DIVIDE;
    return token;
  }
  if (c == ';') {
    token.value[0] = c;
    token.value[1] = '\0';
    token.type = TOKEN_SEMICOLON;
    return token;
  }
  if (c == ',') {
    token.value[0] = c;
    token.value[1] = '\0';
    token.type = TOKEN_COMMA;
    return token;
  }
  if (c == '(') {
    token.value[0] = c;
    token.value[1] = '\0';
    token.type = TOKEN_LPAREN;
    return token;
  }
  if (c == ')') {
    token.value[0] = c;
    token.value[1] = '\0';
    token.type = TOKEN_RPAREN;
    return token;
  }

  /* Build alphanumeric tokens */
  int pos = 0;
  while (c != EOF && !isOpOrDelim(c) && c != ' ' && c != '\t' && c != '\r' &&
         c != '\n' && c != '"' && pos < 255) {
    token.value[pos++] = c;
    c = fgetc(inputFile);
  }
  token.value[pos] = '\0';

  if (c != EOF)
    ungetc(c, inputFile);

  if (pos > 0) {
    /* Check keyword */
    int kwIndex = isKeyword(token.value);
    if (kwIndex >= 0) {
      switch (kwIndex) {
      case 0:
        token.type = TOKEN_VAR;
        break;
      case 1:
        token.type = TOKEN_LOAD;
        break;
      case 2:
        token.type = TOKEN_FILTER;
        break;
      case 3:
        token.type = TOKEN_MAP;
        break;
      case 4:
        token.type = TOKEN_SELECT;
        break;
      case 5:
        token.type = TOKEN_SAVE;
        break;
      case 6:
        token.type = TOKEN_FOR;
        break;
      case 7:
        token.type = TOKEN_WHILE;
        break;
      case 8:
        token.type = TOKEN_IN;
        break;
      case 9:
        token.type = TOKEN_DO;
        break;
      case 10:
        token.type = TOKEN_END;
        break;
      case 11:
        token.type = TOKEN_TO;
        break;
      }
    } else if (validateWithFA(&identifier_fa, token.value)) {
      token.type = TOKEN_IDENTIFIER;
    } else if (validateWithFA(&number_fa, token.value)) {
      token.type = TOKEN_NUMBER;
    } else {
      token.type = TOKEN_ERROR;
    }
  }

  return token;
}

/* Get token type name for grammar matching */
const char *getTokenTypeName(TokenType type) {
  switch (type) {
  case TOKEN_EOF:
    return "$";
  case TOKEN_VAR:
    return "VAR";
  case TOKEN_LOAD:
    return "LOAD";
  case TOKEN_FILTER:
    return "FILTER";
  case TOKEN_MAP:
    return "MAP";
  case TOKEN_SELECT:
    return "SELECT";
  case TOKEN_SAVE:
    return "SAVE";
  case TOKEN_FOR:
    return "FOR";
  case TOKEN_WHILE:
    return "WHILE";
  case TOKEN_IN:
    return "IN";
  case TOKEN_DO:
    return "DO";
  case TOKEN_END:
    return "END";
  case TOKEN_TO:
    return "TO";
  case TOKEN_PLUS:
    return "+";
  case TOKEN_MINUS:
    return "-";
  case TOKEN_MULTIPLY:
    return "*";
  case TOKEN_DIVIDE:
    return "/";
  case TOKEN_ASSIGN:
    return "=";
  case TOKEN_EQ:
    return "==";
  case TOKEN_NEQ:
    return "!=";
  case TOKEN_GT:
    return ">";
  case TOKEN_LT:
    return "<";
  case TOKEN_GTE:
    return ">=";
  case TOKEN_LTE:
    return "<=";
  case TOKEN_SEMICOLON:
    return ";";
  case TOKEN_COMMA:
    return ",";
  case TOKEN_LPAREN:
    return "(";
  case TOKEN_RPAREN:
    return ")";
  case TOKEN_IDENTIFIER:
    return "IDENTIFIER";
  case TOKEN_NUMBER:
    return "NUMBER";
  case TOKEN_STRING:
    return "STRING";
  case TOKEN_ERROR:
    return "ERROR";
  default:
    return "UNKNOWN";
  }
}

/* ========== Simple Tokenizer for Test Sequences ========== */

static FILE *simpleFile = NULL;
static char simpleBuffer[1024];
static char *simpleToken = NULL;

bool initSimpleTokenizer(const char *filename) {
  simpleFile = fopen(filename, "r");
  if (!simpleFile) {
    fprintf(stderr, "Error: Cannot open file '%s'\n", filename);
    return false;
  }

  /* Read entire file */
  simpleBuffer[0] = '\0';
  char line[256];
  while (fgets(line, sizeof(line), simpleFile)) {
    line[strcspn(line, "\n\r")] = ' '; /* Replace newline with space */
    strcat(simpleBuffer, line);
  }

  fclose(simpleFile);
  simpleFile = NULL;

  /* Start tokenizing */
  simpleToken = strtok(simpleBuffer, " \t\n\r");
  return true;
}

const char *getNextSimpleToken() {
  if (!simpleToken)
    return NULL;

  const char *current = simpleToken;
  simpleToken = strtok(NULL, " \t\n\r");
  return current;
}

void cleanupSimpleTokenizer() { simpleToken = NULL; }
