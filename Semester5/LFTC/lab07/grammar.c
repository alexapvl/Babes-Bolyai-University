/*
 * grammar.c - Grammar Loading and Manipulation
 * Lab 7 - LFTC (Formal Languages and Compiler Techniques)
 */

#include "grammar.h"
#include <ctype.h>

/* Create a new grammar structure */
Grammar *createGrammar() {
  Grammar *g = (Grammar *)malloc(sizeof(Grammar));
  if (!g)
    return NULL;

  g->numSymbols = 0;
  g->numTerminals = 0;
  g->numNonTerminals = 0;
  g->numProductions = 0;
  g->startSymbol = -1;

  /* Add epsilon and end marker as special symbols */
  g->epsilonIndex = addSymbol(g, EPSILON, SYMBOL_EPSILON);
  g->endMarkerIndex = addSymbol(g, END_MARKER, SYMBOL_END_MARKER);

  return g;
}

/* Free grammar memory */
void freeGrammar(Grammar *g) {
  if (g)
    free(g);
}

/* Add a symbol to the grammar */
int addSymbol(Grammar *g, const char *name, SymbolType type) {
  /* Check if symbol already exists */
  int existing = findSymbol(g, name);
  if (existing >= 0)
    return existing;

  if (g->numSymbols >= MAX_SYMBOLS) {
    fprintf(stderr, "Error: Maximum symbols exceeded\n");
    return -1;
  }

  int index = g->numSymbols;
  strncpy(g->symbols[index].name, name, MAX_SYMBOL_LEN - 1);
  g->symbols[index].name[MAX_SYMBOL_LEN - 1] = '\0';
  g->symbols[index].type = type;
  g->symbols[index].index = index;
  g->numSymbols++;

  /* Add to appropriate list */
  if (type == SYMBOL_TERMINAL) {
    g->terminals[g->numTerminals++] = index;
  } else if (type == SYMBOL_NONTERMINAL) {
    g->nonTerminals[g->numNonTerminals++] = index;
  }

  return index;
}

/* Find a symbol by name */
int findSymbol(Grammar *g, const char *name) {
  for (int i = 0; i < g->numSymbols; i++) {
    if (strcmp(g->symbols[i].name, name) == 0) {
      return i;
    }
  }
  return -1;
}

/* Add a production rule */
int addProduction(Grammar *g, int lhs, int *rhs, int rhsLen) {
  if (g->numProductions >= MAX_PRODUCTIONS) {
    fprintf(stderr, "Error: Maximum productions exceeded\n");
    return -1;
  }

  int index = g->numProductions;
  g->productions[index].lhsIndex = lhs;
  g->productions[index].rhsLength = rhsLen;
  g->productions[index].productionNumber = index + 1; /* 1-based */

  for (int i = 0; i < rhsLen; i++) {
    g->productions[index].rhs[i] = rhs[i];
  }

  g->numProductions++;
  return index;
}

/* Helper: Check if a string looks like a non-terminal (starts with uppercase or
 * is in <brackets>) */
static bool looksLikeNonTerminal(const char *str) {
  if (str[0] == '<')
    return true; /* <non_terminal> format */
  if (isupper(str[0]) && strlen(str) > 1) {
    /* Multi-char uppercase = probably non-terminal like "program" or
     * "STATEMENT" */
    /* But single uppercase or all-caps short words like "VAR" are terminals */
    bool hasLower = false;
    for (int i = 0; str[i]; i++) {
      if (islower(str[i]))
        hasLower = true;
    }
    return hasLower; /* Has lowercase = non-terminal like "program" */
  }
  return false;
}

/* Load grammar from file
 * Format:
 *   # Comments start with #
 *   TERMINALS: VAR LOAD FILTER ...
 *   NONTERMINALS: program statement ...
 *   START: program
 *   program -> statement program_tail
 *   program_tail -> statement program_tail | epsilon
 */
bool loadGrammarFromFile(Grammar *g, const char *filename) {
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    fprintf(stderr, "Error: Cannot open grammar file '%s'\n", filename);
    return false;
  }

  char line[1024];
  bool terminalsLoaded = false;
  bool nonterminalsLoaded = false;

  while (fgets(line, sizeof(line), fp)) {
    /* Remove trailing newline */
    line[strcspn(line, "\n\r")] = '\0';

    /* Skip empty lines and comments */
    if (line[0] == '\0' || line[0] == '#')
      continue;

    /* Parse TERMINALS line */
    if (strncmp(line, "TERMINALS:", 10) == 0) {
      char *token = strtok(line + 10, " \t");
      while (token) {
        if (strlen(token) > 0) {
          addSymbol(g, token, SYMBOL_TERMINAL);
        }
        token = strtok(NULL, " \t");
      }
      terminalsLoaded = true;
      continue;
    }

    /* Parse NONTERMINALS line */
    if (strncmp(line, "NONTERMINALS:", 13) == 0) {
      char *token = strtok(line + 13, " \t");
      while (token) {
        if (strlen(token) > 0) {
          addSymbol(g, token, SYMBOL_NONTERMINAL);
        }
        token = strtok(NULL, " \t");
      }
      nonterminalsLoaded = true;
      continue;
    }

    /* Parse START line */
    if (strncmp(line, "START:", 6) == 0) {
      char *startName = strtok(line + 6, " \t");
      if (startName) {
        g->startSymbol = findSymbol(g, startName);
        if (g->startSymbol < 0) {
          fprintf(stderr, "Error: Start symbol '%s' not found\n", startName);
        }
      }
      continue;
    }

    /* Parse production rule: LHS -> RHS1 RHS2 ... | ALT1 ALT2 ... */
    char *arrow = strstr(line, "->");
    if (arrow) {
      /* Extract LHS */
      *arrow = '\0';
      char lhsName[MAX_SYMBOL_LEN];
      sscanf(line, "%s", lhsName);

      int lhsIndex = findSymbol(g, lhsName);
      if (lhsIndex < 0) {
        fprintf(stderr, "Error: LHS symbol '%s' not found\n", lhsName);
        continue;
      }

      /* Parse RHS - manually split by | to avoid nested strtok issues */
      char *rhsStr = arrow + 2;

      /* Process each alternative separated by | */
      char *altStart = rhsStr;
      char *altEnd;

      do {
        /* Find next | or end of string */
        altEnd = strchr(altStart, '|');

        /* Copy this alternative to a buffer */
        char altBuffer[256];
        if (altEnd) {
          int len = altEnd - altStart;
          strncpy(altBuffer, altStart, len);
          altBuffer[len] = '\0';
        } else {
          strcpy(altBuffer, altStart);
        }

        /* Parse the alternative (space-separated symbols) */
        int rhs[MAX_RHS_LENGTH];
        int rhsLen = 0;

        char *token = strtok(altBuffer, " \t");
        while (token && rhsLen < MAX_RHS_LENGTH) {
          if (strlen(token) > 0) {
            /* Check for epsilon */
            if (strcmp(token, "epsilon") == 0 || strcmp(token, "ε") == 0 ||
                strcmp(token, "eps") == 0) {
              rhs[rhsLen++] = g->epsilonIndex;
            } else {
              int symIndex = findSymbol(g, token);
              if (symIndex < 0) {
                fprintf(stderr, "Error: Symbol '%s' not found in production\n",
                        token);
              } else {
                rhs[rhsLen++] = symIndex;
              }
            }
          }
          token = strtok(NULL, " \t");
        }

        if (rhsLen > 0) {
          addProduction(g, lhsIndex, rhs, rhsLen);
        }

        /* Move to next alternative */
        altStart = altEnd ? altEnd + 1 : NULL;

      } while (altStart);
    }
  }

  fclose(fp);

  if (!terminalsLoaded || !nonterminalsLoaded) {
    fprintf(stderr, "Warning: TERMINALS or NONTERMINALS section missing\n");
  }

  return true;
}

/* Print grammar for debugging */
void printGrammar(Grammar *g) {
  printf("\n╔════════════════════════════════════════════════════════════╗\n");
  printf("║                    GRAMMAR DEFINITION                       ║\n");
  printf("╚════════════════════════════════════════════════════════════╝\n\n");

  printf("Terminals (%d): ", g->numTerminals);
  for (int i = 0; i < g->numTerminals; i++) {
    printf("%s ", g->symbols[g->terminals[i]].name);
  }
  printf("\n\n");

  printf("Non-terminals (%d): ", g->numNonTerminals);
  for (int i = 0; i < g->numNonTerminals; i++) {
    printf("%s ", g->symbols[g->nonTerminals[i]].name);
  }
  printf("\n\n");

  if (g->startSymbol >= 0) {
    printf("Start symbol: %s\n\n", g->symbols[g->startSymbol].name);
  }

  printf("Productions (%d):\n", g->numProductions);
  printf("─────────────────────────────────────────────────────────────\n");
  for (int i = 0; i < g->numProductions; i++) {
    Production *p = &g->productions[i];
    printf("  %2d. %s -> ", p->productionNumber, g->symbols[p->lhsIndex].name);
    for (int j = 0; j < p->rhsLength; j++) {
      printf("%s ", g->symbols[p->rhs[j]].name);
    }
    printf("\n");
  }
  printf("─────────────────────────────────────────────────────────────\n\n");
}

/* Initialize a symbol set */
void initSet(SymbolSet *set) { set->size = 0; }

/* Add element to set (returns true if added, false if already present) */
bool addToSet(SymbolSet *set, int element) {
  if (setContains(set, element))
    return false;
  if (set->size >= MAX_SET_SIZE) {
    fprintf(stderr, "Error: Set size exceeded\n");
    return false;
  }
  set->elements[set->size++] = element;
  return true;
}

/* Check if set contains element */
bool setContains(SymbolSet *set, int element) {
  for (int i = 0; i < set->size; i++) {
    if (set->elements[i] == element)
      return true;
  }
  return false;
}

/* Union: dest = dest ∪ src (returns true if dest changed) */
bool setUnion(SymbolSet *dest, SymbolSet *src) {
  bool changed = false;
  for (int i = 0; i < src->size; i++) {
    if (addToSet(dest, src->elements[i])) {
      changed = true;
    }
  }
  return changed;
}

/* Union except one element: dest = dest ∪ (src \ {exclude}) */
bool setUnionExcept(SymbolSet *dest, SymbolSet *src, int exclude) {
  bool changed = false;
  for (int i = 0; i < src->size; i++) {
    if (src->elements[i] != exclude) {
      if (addToSet(dest, src->elements[i])) {
        changed = true;
      }
    }
  }
  return changed;
}

/* Print a symbol set */
void printSet(Grammar *g, SymbolSet *set, const char *name) {
  printf("%s = { ", name);
  for (int i = 0; i < set->size; i++) {
    printf("%s", g->symbols[set->elements[i]].name);
    if (i < set->size - 1)
      printf(", ");
  }
  printf(" }\n");
}

/* Check if symbol is terminal */
bool isTerminal(Grammar *g, int symbolIndex) {
  if (symbolIndex < 0 || symbolIndex >= g->numSymbols)
    return false;
  return g->symbols[symbolIndex].type == SYMBOL_TERMINAL ||
         g->symbols[symbolIndex].type == SYMBOL_END_MARKER;
}

/* Check if symbol is non-terminal */
bool isNonTerminal(Grammar *g, int symbolIndex) {
  if (symbolIndex < 0 || symbolIndex >= g->numSymbols)
    return false;
  return g->symbols[symbolIndex].type == SYMBOL_NONTERMINAL;
}

/* Check if symbol is epsilon */
bool isEpsilon(Grammar *g, int symbolIndex) {
  return symbolIndex == g->epsilonIndex;
}

/* Get symbol name */
const char *getSymbolName(Grammar *g, int symbolIndex) {
  if (symbolIndex < 0 || symbolIndex >= g->numSymbols)
    return "?";
  return g->symbols[symbolIndex].name;
}
