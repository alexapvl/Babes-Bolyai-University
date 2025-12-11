# Lab 6: Syntactical Analysis (Parsing) with Yacc

## What is a Production Sequence?

A **production** is a grammar rule that was used to recognize a part of your program.

Think of it like this: When you have a sentence "The cat sleeps", you used these grammar rules:

1. `<sentence> ::= <subject> <verb>`
2. `<subject> ::= "The cat"`
3. `<verb> ::= "sleeps"`

Similarly, when parsing `VAR x = 10;`, the parser uses productions like:

```
<operand> ::= <number>         # Recognized "10" as a number
<expression> ::= <term> ...    # Built an expression from the number
<var_stmt> ::= VAR ...         # Recognized the whole VAR statement
```

**Production sequence** = The list of all grammar rules used to parse your entire program, in the order they were applied.

---

## Lab Requirements & Where They're Implemented

### ✅ Requirement 1: Define Grammar for Mini-DSL

**WHERE**: File `parser.y`, lines 39-196

**WHAT**: All your grammar rules from Lab1's BNF are written in Yacc format.

**CODE EXAMPLE**:

```c
// From parser.y, lines 68-70
var_stmt:
    VAR IDENTIFIER ASSIGN expression SEMICOLON
    { print_production("<var_stmt> ::= VAR <identifier> = <expression> ;"); }
    ;
```

**EXPLANATION**: This says "A var_stmt is the keyword VAR, followed by an identifier, followed by =, followed by an expression, followed by ;"

**ALL GRAMMAR RULES** defined here:

- `program` (line 37)
- `statement` (line 49) - can be var_stmt, load_stmt, filter_stmt, etc.
- `var_stmt` (line 68)
- `load_stmt` (line 73)
- `filter_stmt` (line 78)
- `map_stmt` (line 83)
- `select_stmt` (line 88)
- `save_stmt` (line 93)
- `for_stmt` (line 98)
- `while_stmt` (line 103)
- `expression` (line 108)
- `term` (line 122)
- `factor` (line 136)
- And more...

---

### ✅ Requirement 2: Perform Parsing - Output String of Productions

**WHERE**: File `parser.y`, lines 18-23 and inside each grammar rule

**WHAT**: The `print_production()` function outputs each production as it's used.

**CODE**:

```c
// parser.y, lines 18-23
int production_number = 1;

void print_production(const char* production) {
    printf("%d. %s\n", production_number++, production);
}
```

**HOW IT WORKS**: Inside each grammar rule, when the parser recognizes that pattern, it calls `print_production()`:

```c
// parser.y, lines 68-70
var_stmt:
    VAR IDENTIFIER ASSIGN expression SEMICOLON
    { print_production("<var_stmt> ::= VAR <identifier> = <expression> ;"); }
    ;
```

The part in `{ }` is **action code** - it runs when this rule matches!

**EXAMPLE OUTPUT**:

```
1. <operand> ::= <number>
2. <factor> ::= <operand>
3. <term> ::= <factor> <term_tail>
4. <expression> ::= <term> <expression_tail>
5. <var_stmt> ::= VAR <identifier> = <expression> ;
```

This shows you **step-by-step** how the parser understood your program!

---

### ✅ Requirement 3: Integrate Scanning (Lex) with Parsing (Yacc)

**WHERE**: Two files work together:

- `lexer.l` - provides tokens one at a time
- `parser.y` - validates syntax using those tokens

**THE CONNECTION**: Function `yylex()` in `lexer.l` (line 352)

**HOW THEY CONNECT**:

1. **Parser** (`parser.y` line 242) calls `yyparse()`
2. **yyparse()** internally calls `yylex()` to get next token
3. **Lexer** (`lexer.l` line 352) returns one token code
4. **Parser** uses that token to match against grammar rules
5. Repeat until file ends

**CODE FLOW DIAGRAM**:

```
parser.y (main)
    ↓
  yyparse()
    ↓
  yylex() ─────────────→ lexer.l (line 352)
    ↑                         ↓
    │                    Read characters
    │                    Build token "VAR"
    │                    Validate with FA
    │                    Add to PIF
    │                         ↓
    └─────────────────  return VAR;  (token code)
```

---

## How lexer.l and parser.y Work Together

### The Complete Flow

```
START
  │
  ├─ main() in parser.y (line 218)
  │   │
  │   ├─ initHashTable() ─────────────→ Initialize Lab3 structures
  │   │
  │   ├─ fopen(input_file) ────────────→ Open your program file
  │   │
  │   ├─ yyparse() ───────────────────→ Start parsing
  │   │   │
  │   │   │  [Parser wants next token]
  │   │   │
  │   │   ├─ Call yylex() ────────────→ lexer.l (line 352)
  │   │   │                                  │
  │   │   │                                  ├─ Read chars: 'V', 'A', 'R'
  │   │   │                                  ├─ Build token: "VAR"
  │   │   │                                  ├─ Check: isKeyword("VAR") = true
  │   │   │                                  ├─ addToPIF("VAR", -1)
  │   │   │                                  └─ return VAR; ────────────┐
  │   │   │                                                             │
  │   │   │ <──────────────────────────────────────────────────────────┘
  │   │   │  [Received token: VAR]
  │   │   │
  │   │   ├─ Try to match grammar rules
  │   │   │    ↓
  │   │   │  var_stmt: VAR IDENTIFIER ASSIGN expression SEMICOLON
  │   │   │                  ↑
  │   │   │                  Need IDENTIFIER next...
  │   │   │
  │   │   ├─ Call yylex() again ──────→ lexer.l
  │   │   │                                  │
  │   │   │                                  ├─ Read chars: 'x'
  │   │   │                                  ├─ Build token: "x"
  │   │   │                                  ├─ validateTokenWithFA(&identifier_fa, "x") = true
  │   │   │                                  ├─ addToST("x") → returns 0
  │   │   │                                  ├─ addToPIF("x", 0)
  │   │   │                                  └─ return IDENTIFIER; ─────┐
  │   │   │                                                             │
  │   │   │ <──────────────────────────────────────────────────────────┘
  │   │   │  [Received token: IDENTIFIER]
  │   │   │
  │   │   │ ... continues for '=', '10', ';' ...
  │   │   │
  │   │   ├─ Full rule matched: var_stmt
  │   │   ├─ print_production("<var_stmt> ::= VAR <identifier> = <expression> ;")
  │   │   │
  │   │   └─ Continue until yylex() returns 0 (EOF)
  │   │
  │   ├─ showSymbolTable() ──────────→ Display Lab3 Symbol Table
  │   ├─ showProgramInternalForm() ──→ Display Lab3 PIF
  │   └─ fclose()
  │
END
```

---

## Deep Dive: The Two Key Files

### File 1: `lexer.l` (Modified Lab3 Lexer)

**PURPOSE**: Provide tokens to the parser, one at a time

**KEY FUNCTION**: `yylex()` at line 352

**WHAT IT DOES**:

1. **Reads characters** from input file (manual, character-by-character - same as Lab3)
2. **Builds a token** (e.g., "VAR", "x", "10")
3. **Validates with FA** (same Lab3 finite automata validation)
4. **Adds to PIF and Symbol Table** (same as Lab3)
5. **NEW: Returns token code** to parser (e.g., `return VAR;`)

**CODE STRUCTURE**:

```c
int yylex() {
    // Line 354-367: Initialize on first call
    static FILE* input_file = NULL;
    static int initialized = 0;
    if (!initialized) {
        input_file = yyin;
        identifier_fa = createIdentifierFA();  // Lab3 FA
        number_fa = createNumberFA();          // Lab3 FA
        string_fa = createStringFA();          // Lab3 FA
        initialized = 1;
    }

    // Line 372-382: Skip whitespace (same as Lab3)
    while ((c = fgetc(input_file)) != EOF) {
        if (c == '\n') { lineNumber++; continue; }
        if (c == ' ' || c == '\t' || c == '\r') continue;
        break;
    }

    if (c == EOF) return 0;  // Tell parser: no more tokens

    // Line 388-408: Handle strings (same as Lab3)
    if (c == '"') {
        // ... build string token ...
        if (validateTokenWithFA(&string_fa, token)) {  // Lab3 FA validation
            int stCode = addToST(token);               // Lab3 function
            addToPIF(token, stCode);                   // Lab3 function
            return STRING;  // ← NEW: Return to parser
        }
    }

    // Line 410-419: Handle operators (same as Lab3)
    if (c == '=' || c == '!' || c == '>' || c == '<') {
        // ... build operator token ...
        addToPIF(token, -1);                           // Lab3 function
        return getOperatorToken(token);  // ← NEW: Return to parser
    }

    // Line 442-478: Handle identifiers/keywords/numbers (same as Lab3)
    if (isKeyword(token)) {                            // Lab3 function
        addToPIF(token, -1);                           // Lab3 function
        return getKeywordToken(token);  // ← NEW: Return to parser
    }
    else if (validateTokenWithFA(&identifier_fa, token)) {  // Lab3 FA
        int stCode = addToST(token);                   // Lab3 function
        addToPIF(token, stCode);                       // Lab3 function
        return IDENTIFIER;  // ← NEW: Return to parser
    }
    else if (validateTokenWithFA(&number_fa, token)) { // Lab3 FA
        int stCode = addToST(token);                   // Lab3 function
        addToPIF(token, stCode);                       // Lab3 function
        return NUMBER;  // ← NEW: Return to parser
    }
}
```

**WHAT'S THE SAME AS LAB3?**

- All data structures (HashTable, PIF, FiniteAutomata)
- All FA validation (`validateTokenWithFA`, `createIdentifierFA`, etc.)
- Manual character-by-character reading
- Token building logic
- NO REGEX - pure manual parsing

**WHAT'S NEW?**

- Returns token codes instead of void
- Uses static variables to maintain state between calls
- Helper functions `getKeywordToken()` and `getOperatorToken()` to map strings to codes

---

### File 2: `parser.y` (Yacc Parser)

**PURPOSE**: Check if tokens form valid syntax according to grammar

**KEY SECTIONS**:

#### 1. Header (lines 1-24)

```c
%{
#include <stdio.h>
extern int yylex();                    // Tell parser: lexer provides this
extern int lineNumber;                 // From lexer
extern int errorFound;                 // From lexer
extern void initHashTable();           // From Lab3 lexer
extern void showSymbolTable();         // From Lab3 lexer
extern void showProgramInternalForm(); // From Lab3 lexer

FILE* yyin;  // Input file for lexer

int production_number = 1;
void print_production(const char* production) {
    printf("%d. %s\n", production_number++, production);
}
%}
```

#### 2. Token Definitions (lines 27-32)

```c
%token VAR LOAD FILTER MAP SELECT SAVE FOR WHILE IN DO END TO
%token IDENTIFIER STRING NUMBER
%token PLUS MINUS MULTIPLY DIVIDE
%token ASSIGN SEMICOLON COMMA LPAREN RPAREN
%token EQ NEQ GT LT GTE LTE
```

**EXPLANATION**: These define all the token types the lexer can return.
Yacc automatically assigns numbers: VAR=258, LOAD=259, IDENTIFIER=260, etc.

#### 3. Grammar Rules (lines 39-196)

```c
program:
    statement program_tail
    { print_production("<program> ::= <statement> <program_tail>"); }
    ;

var_stmt:
    VAR IDENTIFIER ASSIGN expression SEMICOLON
    { print_production("<var_stmt> ::= VAR <identifier> = <expression> ;"); }
    ;

expression:
    term expression_tail
    { print_production("<expression> ::= <term> <expression_tail>"); }
    ;
```

**EXPLANATION**: Each rule says "if you see this pattern of tokens, it's valid"
The `{ }` part runs when the rule matches - that's where we print the production!

#### 4. Main Function (lines 218-252)

```c
int main(int argc, char** argv) {
    // Initialize Lab3 structures
    initHashTable();

    // Open input file
    yyin = fopen(argv[1], "r");

    // Parse!
    int result = yyparse();  // ← This calls yylex() internally

    // Show results
    if (result == 0 && errorFound == 0) {
        printf("✓ Parsing completed successfully!\n");
        showSymbolTable();              // Lab3 output
        showProgramInternalForm();      // Lab3 output
    }

    return result || errorFound;
}
```

---

## Example: Parsing `VAR x = 10;` Step-by-Step

### What Happens:

1. **Parser calls `yylex()`**

   - Lexer reads 'V', 'A', 'R'
   - Recognizes keyword
   - Returns `VAR` (token code 258)

2. **Parser receives VAR**

   - Looks for rules starting with VAR
   - Finds: `var_stmt: VAR IDENTIFIER ...`
   - Needs IDENTIFIER next

3. **Parser calls `yylex()` again**

   - Lexer reads 'x'
   - Validates with identifier FA ✓
   - Adds "x" to Symbol Table (index 0)
   - Adds to PIF: `x  0`
   - Returns `IDENTIFIER` (token code 260)

4. **Parser receives IDENTIFIER**

   - `var_stmt` rule continues: `VAR IDENTIFIER ASSIGN ...`
   - Needs ASSIGN next

5. **Parser calls `yylex()` again**

   - Lexer reads '='
   - Returns `ASSIGN` (token code 277)

6. **Parser receives ASSIGN**

   - Needs expression next
   - Calls `yylex()` for '10'

7. **Parser calls `yylex()` again**

   - Lexer reads '1', '0'
   - Validates with number FA ✓
   - Adds "10" to Symbol Table (index 1)
   - Adds to PIF: `10  1`
   - Returns `NUMBER` (token code 262)

8. **Parser receives NUMBER**

   - Matches `operand: NUMBER` → prints production #1
   - Matches `factor: operand` → prints production #2
   - Matches `term: factor ...` → prints production #3
   - Matches `expression: term ...` → prints production #4
   - Back to `var_stmt`, needs SEMICOLON

9. **Parser calls `yylex()` again**

   - Lexer reads ';'
   - Returns `SEMICOLON` (token code 278)

10. **Parser receives SEMICOLON**
    - `var_stmt` rule complete! ✓
    - Prints: `<var_stmt> ::= VAR <identifier> = <expression> ;`

---

## Building and Running

### Build

```bash
cd lab6
make
```

**What happens**:

1. `bison -d parser.y` → generates `parser.tab.c` and `parser.tab.h`
2. `cp lexer.l lexer.c` → copies lexer (needed because of .l extension)
3. `gcc ... parser.tab.c lexer.c` → compiles both together
4. Creates executable `parser`

### Run

```bash
./parser ../lab2/program1.txt
```

### Output

```
=======================================================
   Lab6: Parser with Lab3 Lexer
=======================================================

Production Sequence:                    ← Requirement 2
-------------------------------------------------------
1. <operand> ::= <number>
2. <factor> ::= <operand>
...
62. <program> ::= <statement> <program_tail>
-------------------------------------------------------

✓ Parsing completed successfully!
Total productions used: 62

~~~~~~~ Symbol table (Hash Table) ~~~~~~~  ← From Lab3
Bucket[34]: x(0)
...
~~~~~~~ End ST ~~~~~~~

~~~~~~~ Program internal form ~~~~~~~      ← From Lab3
 VAR  -1
 x  0
...
~~~~~~~ End PIF ~~~~~~~
```

---

## Summary

### What Each File Does

| File       | Purpose                      | Key Function                |
| ---------- | ---------------------------- | --------------------------- |
| `lexer.l`  | Tokenize input (Lab3 logic)  | `yylex()` returns one token |
| `parser.y` | Validate syntax with grammar | `yyparse()` checks rules    |

### What Changed from Lab3

**ONLY ONE THING**: `manualTokenize()` became `yylex()` that returns tokens

**EVERYTHING ELSE**: Same Lab3 code (FA validation, Symbol Table, PIF, manual parsing)

### Requirements Met

✅ **Req 1**: Grammar defined in `parser.y` lines 39-196  
✅ **Req 2**: Productions output by `print_production()` in each rule  
✅ **Req 3**: Lexer (`yylex`) integrated with parser (`yyparse`)

**That's it!** You now have lexical analysis (Lab3) + syntactical analysis (Lab6) working together! 🎉
