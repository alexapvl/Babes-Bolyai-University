/*** Definition Section ***/
%{
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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

/* Hash Table for Symbol Table */
STNode* HashTable[HASH_TABLE_SIZE];
int stLength = 0;  // Total number of unique symbols

PIF ProgramInternalForm[300];
int pifLength = 0;

int errorFound = 0;
int lineNumber = 1;

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

%}

/* Token definitions based on BNF grammar */
IDENTIFIER   [a-zA-Z][a-zA-Z0-9_]*
NUMBER       [0-9]+
STRING       \"[a-zA-Z0-9_ ./]*\"
INVALID_ID   [0-9]+[a-zA-Z][a-zA-Z0-9_]*

/*** Rule Section ***/
%%
                                        /* Keywords */
VAR                                     { addToPIF(yytext, -1); }
LOAD                                    { addToPIF(yytext, -1); }
FILTER                                  { addToPIF(yytext, -1); }
MAP                                     { addToPIF(yytext, -1); }
SELECT                                  { addToPIF(yytext, -1); }
SAVE                                    { addToPIF(yytext, -1); }
FOR                                     { addToPIF(yytext, -1); }
WHILE                                   { addToPIF(yytext, -1); }
IN                                      { addToPIF(yytext, -1); }
DO                                      { addToPIF(yytext, -1); }
END                                     { addToPIF(yytext, -1); }
TO                                      { addToPIF(yytext, -1); }

                                        /* Comparison operators - order matters! */
\==                                     { addToPIF(yytext, -1); }
\!=                                     { addToPIF(yytext, -1); }
\>=                                     { addToPIF(yytext, -1); }
\<=                                     { addToPIF(yytext, -1); }
\>                                      { addToPIF(yytext, -1); }
\<                                      { addToPIF(yytext, -1); }

                                        /* Arithmetic operators */
\+                                      { addToPIF(yytext, -1); }
\-                                      { addToPIF(yytext, -1); }
\*                                      { addToPIF(yytext, -1); }
\/                                      { addToPIF(yytext, -1); }

                                        /* Assignment */
\=                                      { addToPIF(yytext, -1); }

                                        /* Delimiters */
\;                                      { addToPIF(yytext, -1); }
\,                                      { addToPIF(yytext, -1); }
\(                                      { addToPIF(yytext, -1); }
\)                                      { addToPIF(yytext, -1); }

                                        /* Identifiers, numbers, and strings */
{IDENTIFIER}                            { int stCode = addToST(yytext); addToPIF(yytext, stCode); }
{NUMBER}                                { int stCode = addToST(yytext); addToPIF(yytext, stCode); }
{STRING}                                { int stCode = addToST(yytext); addToPIF(yytext, stCode); }
{INVALID_ID}                            { errorFound = 1; printf("Illegal identifier '%s' at line %d (identifiers cannot start with a digit)!\n", yytext, lineNumber); }

                                        /* Whitespace handling */
[\n]                                    { ++lineNumber; }
[ \t\r]+                                { ; /* eat up whitespace */ }

                                        /* Error handling */
.                                       { errorFound = 1; printf("Illegal token '%s' at line %d!\n", yytext, lineNumber); }
%%

/* yywrap() - wraps the above rule section */
int yywrap() { return 1; }

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    /* Initialize the hash table */
    initHashTable();

    FILE *fp;
    fp = fopen(argv[1], "r");
    
    if (fp == NULL) {
        printf("Error: Could not open file %s\n", argv[1]);
        freeHashTable();
        return 1;
    }
    
    /* yyin - takes the file pointer which contains the input */
    yyin = fp;

    /* yylex() - this is the main flex function which runs the Rule Section */ 
    yylex();
    
    fclose(fp);
    
    if (errorFound == 0) {
        showSymbolTable();
        showProgramInternalForm();
        printf("\nLexical analysis completed successfully!\n");
    } else {
        printf("\nLexical analysis failed due to errors.\n");
    }
    
    /* Free the hash table memory */
    freeHashTable();
  
    return errorFound;
}

