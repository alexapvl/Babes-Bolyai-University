/*** Definition Section ***/
#include "parser.tab.h"  /* Token definitions from parser */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern FILE* yyin;

/* Hash Table Configuration */
#define HASH_TABLE_SIZE 101  // Prime number for better distribution

/* Program Internal Form structure */
typedef struct {
    char token[257];
    int symbolTableCode;
} PIF;

/* Symbol Table node structure (for chaining) */
typedef struct STNode {
    char token[257];
    int index;
    struct STNode* next;
} STNode;

// Character class types for transitions
typedef enum {
    CHAR_CLASS_LITERAL,     // Exact character match
    CHAR_CLASS_LETTER,      // Any letter [a-zA-Z]
    CHAR_CLASS_DIGIT,       // Any digit [0-9]
    CHAR_CLASS_NONZERO,     // Non-zero digit [1-9]
    CHAR_CLASS_UNDERSCORE,  // Underscore '_'
    CHAR_CLASS_STRING_CHAR, // Valid string characters [a-zA-Z0-9_ ./]
    CHAR_CLASS_ANY          // Any character
} CharClass;

// Structure to represent a transition in the FA
typedef struct {
    int from_state;
    CharClass char_class;   // Type of character class
    char literal;           // Used when char_class is CHAR_CLASS_LITERAL
    int to_state;
} Transition;

// Structure to represent the Finite Automaton
typedef struct {
    int num_states;
    char* states; // Representing states as Q0, Q1, ...
    int num_alphabet;
    char* alphabet;
    int num_transitions;
    Transition* transitions;
    int start_state;
    int num_final_states;
    int* final_states;
} FiniteAutomata;

/* Hash Table for Symbol Table */
STNode* HashTable[HASH_TABLE_SIZE];
int stLength = 0;  // Total number of unique symbols

PIF ProgramInternalForm[300];
int pifLength = 0;

int errorFound = 0;
int lineNumber = 1;

/* Function Prototypes */
void generateIdentifierGrammar();
void generateNumberGrammar();
void generateStringGrammar();
void generateCarGrammar();
int validateTokenWithFA(FiniteAutomata* fa, const char* input);
void manualTokenize(FILE* fp);

/* FA Creation Functions */
FiniteAutomata createIdentifierFA();
FiniteAutomata createNumberFA();
FiniteAutomata createStringFA();
FiniteAutomata createInvalidIdFA();

/* Global FAs for validation during lexing */
FiniteAutomata identifier_fa;
FiniteAutomata number_fa;
FiniteAutomata string_fa;
FiniteAutomata invalid_id_fa;

/* Keyword list for exact matching */
const char* keywords[] = {"VAR", "LOAD", "FILTER", "MAP", "SELECT", "SAVE", "FOR", "WHILE", "IN", "DO", "END", "TO", NULL};

/* Check if a token is a keyword */
int isKeyword(const char* token) {
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(token, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

/* Check if character is operator or delimiter */
int isOperatorOrDelimiter(char c) {
    return (c == '=' || c == '!' || c == '>' || c == '<' || 
            c == '+' || c == '-' || c == '*' || c == '/' ||
            c == ';' || c == ',' || c == '(' || c == ')');
}

/* Hash function: Simple string hash (djb2 algorithm) */
unsigned int hash(char* str) {
    unsigned long hash = 5381;
    int c;
    
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    
    return hash % HASH_TABLE_SIZE;
}

/* Initialize hash table */
void initHashTable() {
    for(int i = 0; i < HASH_TABLE_SIZE; i++) {
        HashTable[i] = NULL;
    }
}

/* Add token to Symbol Table (Hash Table implementation) */
int addToST(char* token) {
    unsigned int hashIndex = hash(token);
    
    // Search for existing entry in the chain
    STNode* current = HashTable[hashIndex];
    while(current != NULL) {
        if(strcmp(current->token, token) == 0) {
            return current->index;  // Token already exists
        }
        current = current->next;
    }
    
    // Token not found, create new node
    STNode* newNode = (STNode*)malloc(sizeof(STNode));
    strcpy(newNode->token, token);
    newNode->index = stLength;
    
    // Insert at the beginning of the chain (most efficient)
    newNode->next = HashTable[hashIndex];
    HashTable[hashIndex] = newNode;
    
    stLength++;
    return newNode->index;
}

void addToPIF(char* token, int stCode) {
    strcpy(ProgramInternalForm[pifLength].token, token);
    ProgramInternalForm[pifLength].symbolTableCode = stCode;
    pifLength++;
}

void showSymbolTable() {
    printf("~~~~~~~ Symbol table (Hash Table) ~~~~~~~\n");
    printf("Hash Table Size: %d, Total Symbols: %d\n", HASH_TABLE_SIZE, stLength);
    printf("-------------------------------------------\n");
    
    int displayedCount = 0;
    for(int i = 0; i < HASH_TABLE_SIZE; i++) {
        STNode* current = HashTable[i];
        if(current != NULL) {
            printf("Bucket[%d]: ", i);
            while(current != NULL) {
                printf("%s(%d)", current->token, current->index);
                displayedCount++;
                if(current->next != NULL) {
                    printf(" -> ");
                }
                current = current->next;
            }
            printf("\n");
        }
    }
    
    printf("~~~~~~~ End ST ~~~~~~~\n");
    printf("\n");
}

void showProgramInternalForm() {
    printf("~~~~~~~ Program internal form ~~~~~~~\n");
    
    for(int i = 0; i < pifLength; i++) {
        printf(" %s  %d ", ProgramInternalForm[i].token, ProgramInternalForm[i].symbolTableCode);
        printf("\n");
    }
    
    printf("~~~~~~~ End PIF ~~~~~~~\n");
    printf("\n");
}

/* Free hash table memory */
void freeHashTable() {
    for(int i = 0; i < HASH_TABLE_SIZE; i++) {
        STNode* current = HashTable[i];
        while(current != NULL) {
            STNode* temp = current;
            current = current->next;
            free(temp);
        }
        HashTable[i] = NULL;
    }
}

/* Manual tokenization function - character by character, no regex! */
void manualTokenize(FILE* fp) {
    char c;
    char token[257];
    int tokenPos = 0;
    
    while ((c = fgetc(fp)) != EOF) {
        // Handle newlines
        if (c == '\n') {
            lineNumber++;
            continue;
        }
        
        // Skip whitespace
        if (c == ' ' || c == '\t' || c == '\r') {
            continue;
        }
        
        // Handle strings - read until closing quote
        if (c == '"') {
            token[0] = c;
            tokenPos = 1;
            while ((c = fgetc(fp)) != EOF && c != '"') {
                token[tokenPos++] = c;
            }
            if (c == '"') {
                token[tokenPos++] = c;
            }
            token[tokenPos] = '\0';
            
            // Validate with FA
            if (validateTokenWithFA(&string_fa, token)) {
                int stCode = addToST(token);
                addToPIF(token, stCode);
            } else {
                errorFound = 1;
                printf("Invalid string '%s' at line %d!\n", token, lineNumber);
            }
            tokenPos = 0;
            continue;
        }
        
        // Handle two-character operators (==, !=, >=, <=)
        if (c == '=' || c == '!' || c == '>' || c == '<') {
            token[0] = c;
            char next = fgetc(fp);
            if (next == '=') {
                token[1] = '=';
                token[2] = '\0';
                addToPIF(token, -1);
            } else {
                token[1] = '\0';
                addToPIF(token, -1);
                ungetc(next, fp);  // Put back the character
            }
            continue;
        }
        
        // Handle single-character operators and delimiters
        if (c == '+' || c == '-' || c == '*' || c == '/' ||
            c == ';' || c == ',' || c == '(' || c == ')') {
            token[0] = c;
            token[1] = '\0';
            addToPIF(token, -1);
            continue;
        }
        
        // Build alphanumeric tokens character by character
        tokenPos = 0;
        while (c != EOF && !isOperatorOrDelimiter(c) && 
               c != ' ' && c != '\t' && c != '\r' && c != '\n' && c != '"') {
            token[tokenPos++] = c;
            c = fgetc(fp);
        }
        token[tokenPos] = '\0';
        
        // Put back the delimiter/whitespace we just read
        if (c != EOF) {
            ungetc(c, fp);
        }
        
        if (tokenPos > 0) {
            // Check if it's a keyword
            if (isKeyword(token)) {
                addToPIF(token, -1);
            }
            // Try FAs - identifier, number, or invalid ID
            else if (validateTokenWithFA(&identifier_fa, token)) {
                int stCode = addToST(token);
                addToPIF(token, stCode);
            } else if (validateTokenWithFA(&number_fa, token)) {
                int stCode = addToST(token);
                addToPIF(token, stCode);
            } else if (validateTokenWithFA(&invalid_id_fa, token)) {
                errorFound = 1;
                printf("Illegal identifier '%s' at line %d (identifiers cannot start with a digit)!\n", token, lineNumber);
            } else {
                errorFound = 1;
                printf("Invalid token '%s' at line %d!\n", token, lineNumber);
            }
        }
    }
}

/* yywrap() - wraps the above rule section */
int yywrap() { return 1; }

/* Helper function to map keyword string to token code */
int getKeywordToken(const char* token) {
    if (strcmp(token, "VAR") == 0) return VAR;
    if (strcmp(token, "LOAD") == 0) return LOAD;
    if (strcmp(token, "FILTER") == 0) return FILTER;
    if (strcmp(token, "MAP") == 0) return MAP;
    if (strcmp(token, "SELECT") == 0) return SELECT;
    if (strcmp(token, "SAVE") == 0) return SAVE;
    if (strcmp(token, "FOR") == 0) return FOR;
    if (strcmp(token, "WHILE") == 0) return WHILE;
    if (strcmp(token, "IN") == 0) return IN;
    if (strcmp(token, "DO") == 0) return DO;
    if (strcmp(token, "END") == 0) return END;
    if (strcmp(token, "TO") == 0) return TO;
    return -1;
}

/* Helper function to map operator string to token code */
int getOperatorToken(const char* token) {
    if (strcmp(token, "+") == 0) return PLUS;
    if (strcmp(token, "-") == 0) return MINUS;
    if (strcmp(token, "*") == 0) return MULTIPLY;
    if (strcmp(token, "/") == 0) return DIVIDE;
    if (strcmp(token, "=") == 0) return ASSIGN;
    if (strcmp(token, ";") == 0) return SEMICOLON;
    if (strcmp(token, ",") == 0) return COMMA;
    if (strcmp(token, "(") == 0) return LPAREN;
    if (strcmp(token, ")") == 0) return RPAREN;
    if (strcmp(token, "==") == 0) return EQ;
    if (strcmp(token, "!=") == 0) return NEQ;
    if (strcmp(token, ">") == 0) return GT;
    if (strcmp(token, "<") == 0) return LT;
    if (strcmp(token, ">=") == 0) return GTE;
    if (strcmp(token, "<=") == 0) return LTE;
    return -1;
}

/* MODIFIED: yylex() function for parser integration */
/* Returns ONE token at a time instead of processing entire file */
int yylex() {
    static FILE* input_file = NULL;
    static int initialized = 0;
    char token[257];
    int tokenPos = 0;
    
    /* First call: initialize */
    if (!initialized) {
        extern FILE* yyin;
        input_file = yyin;
        identifier_fa = createIdentifierFA();
        number_fa = createNumberFA();
        string_fa = createStringFA();
        invalid_id_fa = createInvalidIdFA();
        initialized = 1;
    }
    
    char c;
    
    /* Skip whitespace and handle newlines - SAME AS LAB3 */
    while ((c = fgetc(input_file)) != EOF) {
        if (c == '\n') {
            lineNumber++;
            continue;
        }
        if (c == ' ' || c == '\t' || c == '\r') {
            continue;
        }
        break;
    }
    
    if (c == EOF) {
        return 0;  /* Signal end of input to parser */
    }
    
    /* Handle strings - SAME AS LAB3 */
    if (c == '"') {
        token[0] = c;
        tokenPos = 1;
        while ((c = fgetc(input_file)) != EOF && c != '"') {
            token[tokenPos++] = c;
        }
        if (c == '"') {
            token[tokenPos++] = c;
        }
        token[tokenPos] = '\0';
        
        if (validateTokenWithFA(&string_fa, token)) {
            int stCode = addToST(token);
            addToPIF(token, stCode);
            return STRING;  /* NEW: Return token code */
        } else {
            errorFound = 1;
            printf("Invalid string '%s' at line %d!\n", token, lineNumber);
            return -1;
        }
    }
    
    /* Handle two-character operators - SAME AS LAB3 */
    if (c == '=' || c == '!' || c == '>' || c == '<') {
        token[0] = c;
        char next = fgetc(input_file);
        if (next == '=') {
            token[1] = '=';
            token[2] = '\0';
        } else {
            token[1] = '\0';
            ungetc(next, input_file);
        }
        addToPIF(token, -1);
        return getOperatorToken(token);  /* NEW: Return token code */
    }
    
    /* Handle single-character operators - SAME AS LAB3 */
    if (c == '+' || c == '-' || c == '*' || c == '/' ||
        c == ';' || c == ',' || c == '(' || c == ')') {
        token[0] = c;
        token[1] = '\0';
        addToPIF(token, -1);
        return getOperatorToken(token);  /* NEW: Return token code */
    }
    
    /* Build alphanumeric tokens - SAME AS LAB3 */
    tokenPos = 0;
    while (c != EOF && !isOperatorOrDelimiter(c) && 
           c != ' ' && c != '\t' && c != '\r' && c != '\n' && c != '"') {
        token[tokenPos++] = c;
        c = fgetc(input_file);
    }
    token[tokenPos] = '\0';
    
    if (c != EOF) {
        ungetc(c, input_file);
    }
    
    if (tokenPos > 0) {
        /* Check if keyword - SAME AS LAB3 */
        if (isKeyword(token)) {
            addToPIF(token, -1);
            return getKeywordToken(token);  /* NEW: Return token code */
        }
        /* Validate identifier - SAME AS LAB3 */
        else if (validateTokenWithFA(&identifier_fa, token)) {
            int stCode = addToST(token);
            addToPIF(token, stCode);
            return IDENTIFIER;  /* NEW: Return token code */
        }
        /* Validate number - SAME AS LAB3 */
        else if (validateTokenWithFA(&number_fa, token)) {
            int stCode = addToST(token);
            addToPIF(token, stCode);
            return NUMBER;  /* NEW: Return token code */
        }
        /* Check for invalid identifiers - SAME AS LAB3 */
        else if (validateTokenWithFA(&invalid_id_fa, token)) {
            errorFound = 1;
            printf("Illegal identifier '%s' at line %d (identifiers cannot start with a digit)!\n", token, lineNumber);
            return -1;
        }
        else {
            errorFound = 1;
            printf("Invalid token '%s' at line %d!\n", token, lineNumber);
            return -1;
        }
    }
    
    return yylex();  /* Continue to next token */
}

// Helper function to check if a character matches a character class
int matchesCharClass(char c, CharClass char_class, char literal) {
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
                   (c >= '0' && c <= '9') || c == '_' || c == ' ' ||
                   c == '.' || c == '/';
        case CHAR_CLASS_ANY:
            return 1;
        default:
            return 0;
    }
}

// Generic FA validator function - simulates the FA for a given input string
// Returns 1 if the string is accepted by the FA, 0 otherwise
// 
// OPTIMIZATION: This version skips transitions not from the current state early.
// A production implementation would use a transition table indexed by state
// (e.g., transitions[state][symbol] -> next_state) for O(1) lookups.
int validateTokenWithFA(FiniteAutomata* fa, const char* input) {
    int current_state = fa->start_state;
    int input_length = strlen(input);
    
    // Process each character in the input
    for (int i = 0; i < input_length; i++) {
        char current_char = input[i];
        int found_transition = 0;
        
        // Find a valid transition for the current character
        // We iterate through all transitions, but skip irrelevant ones early
        for (int j = 0; j < fa->num_transitions; j++) {
            Transition t = fa->transitions[j];
            
            // Skip transitions that don't originate from current state
            // This optimization avoids checking character matches for irrelevant transitions
            if (t.from_state != current_state) {
                continue;  // Skip to next transition immediately
            }
            
            // Now check if this transition matches the current character
            if (matchesCharClass(current_char, t.char_class, t.literal)) {
                current_state = t.to_state;
                found_transition = 1;
                break;  // Found a valid transition, move to next character
            }
        }
        
        // If no valid transition found, reject the string
        if (!found_transition) {
            return 0;
        }
    }
    
    // Check if we ended in a final state
    for (int i = 0; i < fa->num_final_states; i++) {
        if (current_state == fa->final_states[i]) {
            return 1;  // String is accepted
        }
    }
    
    return 0;  // Not in a final state, reject
}

// Helper function to get string representation of character class
const char* getCharClassName(CharClass char_class, char literal) {
    static char buf[4];  // Static buffer for literal characters
    
    switch (char_class) {
        case CHAR_CLASS_LITERAL:
            snprintf(buf, sizeof(buf), "%c", literal);
            return buf;
        case CHAR_CLASS_LETTER:
            return "L";
        case CHAR_CLASS_DIGIT:
            return "d";
        case CHAR_CLASS_NONZERO:
            return "D";
        case CHAR_CLASS_UNDERSCORE:
            return "_";
        case CHAR_CLASS_STRING_CHAR:
            return "C";
        case CHAR_CLASS_ANY:
            return "*";
        default:
            return "?";
    }
}

// Function to generate and print the regular grammar from an FA
void generateRegularGrammarFromFiniteAutomata(FiniteAutomata fa, const char* name) {
    printf("\n--- Regular Grammar for %s ---\n", name);
    // Non-terminals are the states
    printf("N = { ");
    for (int i = 0; i < fa.num_states; i++) {
        printf("Q%d%s", i, (i == fa.num_states - 1) ? " " : ", ");
    }
    printf("}\n");

    // Terminals are the alphabet
    printf("T = { %s }\n", fa.alphabet);

    // Start symbol is the start state
    printf("S = Q%d\n", fa.start_state);

    // Production rules
    printf("P = {\n");
    for (int i = 0; i < fa.num_transitions; i++) {
        Transition t = fa.transitions[i];
        const char* symbol = getCharClassName(t.char_class, t.literal);
        printf("    Q%d -> %s Q%d\n", t.from_state, symbol, t.to_state);
    }

    // Rules for final states
    for (int i = 0; i < fa.num_final_states; i++) {
        printf("    Q%d -> ε\n", fa.final_states[i]);
    }
    printf("}\n---------------------------------\n");
}

// Helper function to test and display validation results
void testFAValidation(FiniteAutomata* fa, const char* test_string, const char* fa_name) {
    int result = validateTokenWithFA(fa, test_string);
    printf("  Testing \"%s\": %s\n", test_string, result ? "✓ ACCEPTED" : "✗ REJECTED");
}

// Creates and returns the FA for identifiers
FiniteAutomata createIdentifierFA() {
    // FA for IDENTIFIER: [a-zA-Z][a-zA-Z0-9_]*
    static Transition id_transitions[] = {
        {0, CHAR_CLASS_LETTER, '\0', 1},     // First character must be a letter
        {1, CHAR_CLASS_LETTER, '\0', 1},     // Can have more letters
        {1, CHAR_CLASS_DIGIT, '\0', 1},      // Can have digits
        {1, CHAR_CLASS_UNDERSCORE, '\0', 1}  // Can have underscores
    };
    static int id_final_states[] = {1};

    FiniteAutomata fa = {
        .num_states = 2,
        .states = (char[]){'Q', '0', ',', 'Q', '1', '\0'},
        .num_alphabet = 3,
        .alphabet = "L (letter), d (digit), _ (underscore)",
        .num_transitions = 4,
        .transitions = id_transitions,
        .start_state = 0,
        .num_final_states = 1,
        .final_states = id_final_states
    };
    
    return fa;
}

// Defines and generates the grammar for the Identifier FA
void generateIdentifierGrammar() {
    FiniteAutomata identifier_fa = createIdentifierFA();
    
    generateRegularGrammarFromFiniteAutomata(identifier_fa, "Identifiers");
    
    // Demonstrate validation with test cases
    printf("\nValidation Examples:\n");
    testFAValidation(&identifier_fa, "myVar", "Identifiers");
    testFAValidation(&identifier_fa, "x", "Identifiers");
    testFAValidation(&identifier_fa, "variable_123", "Identifiers");
    testFAValidation(&identifier_fa, "123abc", "Identifiers");  // Should reject (starts with digit)
    testFAValidation(&identifier_fa, "_invalid", "Identifiers"); // Should reject (starts with underscore)
}

// Creates and returns the FA for numbers
FiniteAutomata createNumberFA() {
    // FA for NUMBER: 0 | [1-9][0-9]*
    static Transition num_transitions[] = {
        {0, CHAR_CLASS_LITERAL, '0', 1},   // State 1 is a final state for the number 0
        {0, CHAR_CLASS_NONZERO, '\0', 2},  // Non-zero digit (1-9), moves to state 2
        {2, CHAR_CLASS_DIGIT, '\0', 2}     // Any digit (0-9) can continue
    };
    static int numbers_final_states[] = {1, 2};

    FiniteAutomata fa = {
        .num_states = 3,
        .states = (char[]){'Q', '0', ',', 'Q', '1', ',', 'Q', '2', '\0'},
        .num_alphabet = 3,
        .alphabet = "0 (literal zero), D (non-zero digit), d (any digit)",
        .num_transitions = 3,
        .transitions = num_transitions,
        .start_state = 0,
        .num_final_states = 2,
        .final_states = numbers_final_states
    };
    
    return fa;
}

// Defines and generates the grammar for the Number FA
void generateNumberGrammar() {
    FiniteAutomata number_fa = createNumberFA();
    
    generateRegularGrammarFromFiniteAutomata(number_fa, "Numbers");
    
    // Demonstrate validation with test cases
    printf("\nValidation Examples:\n");
    testFAValidation(&number_fa, "0", "Numbers");
    testFAValidation(&number_fa, "42", "Numbers");
    testFAValidation(&number_fa, "123456", "Numbers");
    testFAValidation(&number_fa, "007", "Numbers");      // Should reject (leading zeros)
    testFAValidation(&number_fa, "00", "Numbers");       // Should reject (multiple zeros)
    testFAValidation(&number_fa, "abc", "Numbers");      // Should reject (not a number)
}

// Creates and returns the FA for strings
FiniteAutomata createStringFA() {
    // FA for STRING: \"[a-zA-Z0-9_ ./]*\"
    static Transition string_transitions[] = {
        {0, CHAR_CLASS_LITERAL, '"', 1},         // Opening quote
        {1, CHAR_CLASS_STRING_CHAR, '\0', 1},    // Any valid character inside the string
        {1, CHAR_CLASS_LITERAL, '"', 2}          // Closing quote
    };
    static int string_final_states[] = {2};

    FiniteAutomata fa = {
        .num_states = 3,
        .states = (char[]){'Q', '0', ',', 'Q', '1', ',', 'Q', '2', '\0'},
        .num_alphabet = 2,
        .alphabet = "\" (quote), C (string char: a-zA-Z0-9_ ./)",
        .num_transitions = 3,
        .transitions = string_transitions,
        .start_state = 0,
        .num_final_states = 1,
        .final_states = string_final_states
    };

    return fa;
}

// Defines and generates the grammar for the String FA
void generateStringGrammar() {
    FiniteAutomata string_fa = createStringFA();

    generateRegularGrammarFromFiniteAutomata(string_fa, "Strings");
    
    // Demonstrate validation with test cases
    printf("\nValidation Examples:\n");
    testFAValidation(&string_fa, "\"hello\"", "Strings");
    testFAValidation(&string_fa, "\"test 123\"", "Strings");
    testFAValidation(&string_fa, "\"file.txt\"", "Strings");
    testFAValidation(&string_fa, "\"unclosed", "Strings");    // Should reject (no closing quote)
    testFAValidation(&string_fa, "noQuotes", "Strings");     // Should reject (no quotes)
    testFAValidation(&string_fa, "\"\"", "Strings");         // Should accept (empty string)
}

// Creates and returns the FA for invalid identifiers (starting with digits)
FiniteAutomata createInvalidIdFA() {
    // FA for INVALID_ID: [0-9]+[a-zA-Z][a-zA-Z0-9_]*
    // Recognizes identifiers that start with digits (which are invalid)
    static Transition invalid_id_transitions[] = {
        {0, CHAR_CLASS_DIGIT, '\0', 1},        // Start with at least one digit
        {1, CHAR_CLASS_DIGIT, '\0', 1},        // Can have more digits
        {1, CHAR_CLASS_LETTER, '\0', 2},       // Then must have a letter
        {2, CHAR_CLASS_LETTER, '\0', 2},       // Can have more letters
        {2, CHAR_CLASS_DIGIT, '\0', 2},        // Can have digits
        {2, CHAR_CLASS_UNDERSCORE, '\0', 2}    // Can have underscores
    };
    static int invalid_id_final_states[] = {2};

    FiniteAutomata fa = {
        .num_states = 3,
        .states = (char[]){'Q', '0', ',', 'Q', '1', ',', 'Q', '2', '\0'},
        .num_alphabet = 3,
        .alphabet = "d (digit), L (letter), _ (underscore)",
        .num_transitions = 6,
        .transitions = invalid_id_transitions,
        .start_state = 0,
        .num_final_states = 1,
        .final_states = invalid_id_final_states
    };
    
    return fa;
}

// Example FA for "car" pattern: car[0-9]+
// This demonstrates that adding a new FA requires ZERO new code!
// We only define the data structure - the same validation logic works!
void generateCarGrammar() {
    // FA for CAR: c a r [0-9]+
    // This pattern matches strings like: car1, car42, car999, etc.
    Transition car_transitions[] = {
        {0, CHAR_CLASS_LITERAL, 'c', 1},     // Must start with 'c'
        {1, CHAR_CLASS_LITERAL, 'a', 2},     // Then 'a'
        {2, CHAR_CLASS_LITERAL, 'r', 3},     // Then 'r'
        {3, CHAR_CLASS_DIGIT, '\0', 4},      // Then at least one digit
        {4, CHAR_CLASS_DIGIT, '\0', 4}       // Can have more digits
    };
    int car_final_states[] = {4};

    FiniteAutomata car_fa = {
        .num_states = 5,
        .states = (char[]){'Q', '0', ',', 'Q', '1', ',', 'Q', '2', ',', 'Q', '3', ',', 'Q', '4', '\0'},
        .num_alphabet = 2,
        .alphabet = "c, a, r (literals), d (digit)",
        .num_transitions = 5,
        .transitions = car_transitions,
        .start_state = 0,
        .num_final_states = 1,
        .final_states = car_final_states
    };

    generateRegularGrammarFromFiniteAutomata(car_fa, "Car Pattern");
    
    // Demonstrate validation with test cases
    printf("\nValidation Examples:\n");
    testFAValidation(&car_fa, "car1", "Car Pattern");
    testFAValidation(&car_fa, "car42", "Car Pattern");
    testFAValidation(&car_fa, "car123", "Car Pattern");
    testFAValidation(&car_fa, "car", "Car Pattern");        // Should reject (no digits)
    testFAValidation(&car_fa, "plane5", "Car Pattern");     // Should reject (wrong prefix)
    testFAValidation(&car_fa, "carr99", "Car Pattern");     // Should reject (extra 'r')
    testFAValidation(&car_fa, "Car1", "Car Pattern");       // Should reject (uppercase 'C')
}

