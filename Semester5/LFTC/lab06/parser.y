%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex();
extern int lineNumber;
extern int errorFound;
FILE* yyin = NULL;

void yyerror(const char* s);

/* Functions from Lab3 lexer */
extern void initHashTable();
extern void showSymbolTable();
extern void showProgramInternalForm();
extern void freeHashTable();

/* Production tracking */
int production_number = 1;

void print_production(const char* production) {
    printf("%d. %s\n", production_number++, production);
}

%}

/* Token definitions */
%token VAR LOAD FILTER MAP SELECT SAVE FOR WHILE IN DO END TO
%token IDENTIFIER STRING NUMBER
%token PLUS MINUS MULTIPLY DIVIDE
%token ASSIGN SEMICOLON COMMA LPAREN RPAREN
%token EQ NEQ GT LT GTE LTE

/* Operator precedence */
%left PLUS MINUS
%left MULTIPLY DIVIDE
%right UMINUS

%%

/* Grammar rules from Lab1 BNF */

program:
    statement program_tail
    { print_production("<program> ::= <statement> <program_tail>"); }
    ;

program_tail:
    statement program_tail
    { print_production("<program_tail> ::= <statement> <program_tail>"); }
    | /* epsilon */
    { print_production("<program_tail> ::= epsilon"); }
    ;

statement:
    var_stmt
    { print_production("<statement> ::= <var_stmt>"); }
    | load_stmt
    { print_production("<statement> ::= <load_stmt>"); }
    | filter_stmt
    { print_production("<statement> ::= <filter_stmt>"); }
    | map_stmt
    { print_production("<statement> ::= <map_stmt>"); }
    | select_stmt
    { print_production("<statement> ::= <select_stmt>"); }
    | save_stmt
    { print_production("<statement> ::= <save_stmt>"); }
    | for_stmt
    { print_production("<statement> ::= <for_stmt>"); }
    | while_stmt
    { print_production("<statement> ::= <while_stmt>"); }
    ;

var_stmt:
    VAR IDENTIFIER ASSIGN expression SEMICOLON
    { print_production("<var_stmt> ::= VAR <identifier> = <expression> ;"); }
    ;

load_stmt:
    LOAD STRING SEMICOLON
    { print_production("<load_stmt> ::= LOAD <string> ;"); }
    ;

filter_stmt:
    FILTER expression comparison_op expression SEMICOLON
    { print_production("<filter_stmt> ::= FILTER <expression> <comparison_op> <expression> ;"); }
    ;

map_stmt:
    MAP operand ASSIGN expression SEMICOLON
    { print_production("<map_stmt> ::= MAP <operand> = <expression> ;"); }
    ;

select_stmt:
    SELECT identifier_list SEMICOLON
    { print_production("<select_stmt> ::= SELECT <identifier_list> ;"); }
    ;

save_stmt:
    SAVE STRING SEMICOLON
    { print_production("<save_stmt> ::= SAVE <string> ;"); }
    ;

for_stmt:
    FOR IDENTIFIER IN range DO statement_list END SEMICOLON
    { print_production("<for_stmt> ::= FOR <identifier> IN <range> DO <statement_list> END ;"); }
    ;

while_stmt:
    WHILE condition DO statement_list END SEMICOLON
    { print_production("<while_stmt> ::= WHILE <condition> DO <statement_list> END ;"); }
    ;

expression:
    term expression_tail
    { print_production("<expression> ::= <term> <expression_tail>"); }
    ;

expression_tail:
    PLUS term expression_tail
    { print_production("<expression_tail> ::= + <term> <expression_tail>"); }
    | MINUS term expression_tail
    { print_production("<expression_tail> ::= - <term> <expression_tail>"); }
    | /* epsilon */
    { print_production("<expression_tail> ::= epsilon"); }
    ;

term:
    factor term_tail
    { print_production("<term> ::= <factor> <term_tail>"); }
    ;

term_tail:
    MULTIPLY factor term_tail
    { print_production("<term_tail> ::= * <factor> <term_tail>"); }
    | DIVIDE factor term_tail
    { print_production("<term_tail> ::= / <factor> <term_tail>"); }
    | /* epsilon */
    { print_production("<term_tail> ::= epsilon"); }
    ;

factor:
    operand
    { print_production("<factor> ::= <operand>"); }
    | LPAREN expression RPAREN
    { print_production("<factor> ::= ( <expression> )"); }
    | MINUS factor %prec UMINUS
    { print_production("<factor> ::= - <factor>"); }
    ;

statement_list:
    statement statement_list
    { print_production("<statement_list> ::= <statement> <statement_list>"); }
    | /* epsilon */
    { print_production("<statement_list> ::= epsilon"); }
    ;

range:
    expression TO expression
    { print_production("<range> ::= <expression> TO <expression>"); }
    ;

condition:
    expression comparison_op expression
    { print_production("<condition> ::= <expression> <comparison_op> <expression>"); }
    ;

operand:
    IDENTIFIER
    { print_production("<operand> ::= <identifier>"); }
    | STRING
    { print_production("<operand> ::= <string>"); }
    | NUMBER
    { print_production("<operand> ::= <number>"); }
    ;

identifier_list:
    IDENTIFIER identifier_list_tail
    { print_production("<identifier_list> ::= <identifier> <identifier_list_tail>"); }
    ;

identifier_list_tail:
    COMMA IDENTIFIER identifier_list_tail
    { print_production("<identifier_list_tail> ::= , <identifier> <identifier_list_tail>"); }
    | /* epsilon */
    { print_production("<identifier_list_tail> ::= epsilon"); }
    ;

comparison_op:
    EQ
    { print_production("<comparison_op> ::= =="); }
    | NEQ
    { print_production("<comparison_op> ::= !="); }
    | GT
    { print_production("<comparison_op> ::= >"); }
    | LT
    { print_production("<comparison_op> ::= <"); }
    | GTE
    { print_production("<comparison_op> ::= >="); }
    | LTE
    { print_production("<comparison_op> ::= <="); }
    ;

%%

void yyerror(const char* s) {
    fprintf(stderr, "Parse error at line %d: %s\n", lineNumber, s);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    /* Initialize Lab3 structures */
    initHashTable();

    /* Open input file */
    extern FILE* yyin;
    yyin = fopen(argv[1], "r");
    if (yyin == NULL) {
        printf("Error: Could not open file %s\n", argv[1]);
        return 1;
    }

    printf("=======================================================\n");
    printf("   Lab6: Parser with Lab3 Lexer\n");
    printf("=======================================================\n");
    printf("\nProduction Sequence:\n");
    printf("-------------------------------------------------------\n");

    /* Parse the input */
    int result = yyparse();

    printf("-------------------------------------------------------\n");
    
    if (result == 0 && errorFound == 0) {
        printf("\n✓ Parsing completed successfully!\n");
        printf("Total productions used: %d\n", production_number - 1);
        
        /* Show Lab3 outputs */
        showSymbolTable();
        showProgramInternalForm();
    } else {
        printf("\n✗ Parsing failed with errors.\n");
    }
    printf("=======================================================\n");

    fclose(yyin);
    freeHashTable();
    return result || errorFound;
}

