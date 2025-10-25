# Lexical Analyzer - Lab 03

This directory contains a lexical analyzer (lexer) built with **Flex** for the custom programming language defined in `lab01/bnf.txt`.

## What is Flex?

Flex (Fast Lexical Analyzer) is a tool for generating lexical analyzers (scanners/tokenizers). It reads a specification file (`.l` file) and generates C code that recognizes patterns in text.

---

## Files

- **`lexer.l`** - Flex specification file (source code)
- **`lex.yy.c`** - Generated C code (created by flex, don't edit manually)
- **`lexer`** - Compiled executable

---

## Prerequisites

Make sure you have:

- **Flex** installed (check with `flex --version`)
- **GCC** compiler installed (check with `gcc --version`)

On macOS, these come with Xcode Command Line Tools.

---

## How to Compile

### Step 1: Generate C Code from Flex File

```bash
flex lexer.l
```

This reads `lexer.l` and generates `lex.yy.c` (a C source file).

### Step 2: Compile the C Code

```bash
gcc lex.yy.c -o lexer
```

This compiles `lex.yy.c` into an executable named `lexer`.

### Quick Compile (Both Steps)

```bash
flex lexer.l && gcc lex.yy.c -o lexer
```

---

## How to Run

```bash
./lexer <input_file>
```

### Examples

Test with the example programs from lab02:

```bash
./lexer ../lab02/program1.txt
```

```bash
./lexer ../lab02/program2.txt
```

---

## Output

The lexer produces two main outputs:

### 1. Symbol Table (ST)

Lists all **identifiers**, **numbers**, and **strings** found in the program with their unique index.

Example:

```
~~~~~~~ Symbol table ~~~~~~~
 x  0
 10  1
 y  2
 "data.csv"  3
~~~~~~~ End ST ~~~~~~~
```

### 2. Program Internal Form (PIF)

Shows the sequence of tokens with references to the Symbol Table.

- **Keywords and operators** have code `-1` (they are reserved/fixed)
- **Identifiers, numbers, and strings** have their Symbol Table index

Example:

```
~~~~~~~ Program internal form ~~~~~~~
 VAR  -1
 x  0
 =  -1
 10  1
 ;  -1
~~~~~~~ End PIF ~~~~~~~
```

---

## Recognized Tokens

### Keywords

```
VAR, LOAD, FILTER, MAP, SELECT, SAVE, FOR, WHILE, IN, DO, END, TO
```

### Operators

**Comparison operators:**

```
==, !=, <, >, <=, >=
```

**Arithmetic operators:**

```
+, -, *, /
```

**Assignment:**

```
=
```

### Delimiters

```
; , ( )
```

### Identifiers

- Must start with a letter (a-z, A-Z)
- Can contain letters, digits, and underscores
- Examples: `x`, `threshold`, `sales_data`, `max_value`

### Numbers

- Integer numbers only
- Examples: `0`, `10`, `200`, `1234`

### Strings

- Enclosed in double quotes
- Can contain letters, digits, underscores, spaces, dots, and slashes
- Examples: `"data.csv"`, `"sales_data.csv"`, `"output file.txt"`

---

## Error Handling

If the lexer encounters an illegal token, it will:

1. Print an error message with the token and line number
2. Continue scanning
3. Skip printing the Symbol Table and PIF
4. Exit with error code

Example error:

```
Illegal token '@' at line 5!
```

---

## Testing Your Own Programs

Create a new program file following the grammar in `lab01/bnf.txt`:

```bash
# Create a test file
nano test_program.txt

# Run the lexer
./lexer test_program.txt
```

---

## Workflow Summary

```bash
# 1. Edit the flex file (if needed)
nano lexer.l

# 2. Compile
flex lexer.l
gcc lex.yy.c -o lexer

# 3. Test
./lexer ../lab02/program1.txt
```

---

## Common Issues

### Issue: `flex: command not found`

**Solution:** Install Xcode Command Line Tools:

```bash
xcode-select --install
```

### Issue: `gcc: command not found`

**Solution:** Same as above - install Xcode Command Line Tools.

### Issue: `Error: Could not open file`

**Solution:** Check that the file path is correct. Use relative or absolute paths.

### Issue: Compilation warnings

**Solution:** The generated `lex.yy.c` may produce some warnings - these are usually safe to ignore.

---

## Understanding the Lexer Structure

The `lexer.l` file has three sections:

### 1. Definition Section

```c
%{
    // C code: includes, data structures, helper functions
%}

// Token pattern definitions (regex patterns)
IDENTIFIER   [a-zA-Z][a-zA-Z0-9_]*
NUMBER       [0-9]+
```

### 2. Rules Section

```
%%
    pattern1    { action1; }
    pattern2    { action2; }
%%
```

### 3. User Code Section

```c
int main() {
    // Program entry point
}
```

---

## Tools Used

### Flex (Fast Lexical Analyzer Generator)

**Purpose:** Generates lexical analyzers from pattern specifications.

**How it works:**

1. Reads the `.l` specification file containing:
   - Regular expression patterns for tokens
   - Actions to perform when patterns are matched
2. Generates optimized C code (`lex.yy.c`) that implements a finite automaton
3. The generated code scans input text and recognizes tokens

**Key Flex Functions:**

- `yylex()` - Main scanning function that processes input
- `yytext` - Global variable containing the matched token text
- `yyin` - Input file pointer
- `yywrap()` - Called when end of file is reached (returns 1 to stop)

**Advantages:**

- Fast pattern matching using DFA (Deterministic Finite Automaton)
- Automatic handling of longest match rule
- Efficient code generation
- Standard tool in compiler construction

### GCC (GNU Compiler Collection)

**Purpose:** Compiles the generated C code into an executable.

**How it works:**

- Takes `lex.yy.c` as input
- Produces machine code executable (`lexer`)
- Links standard C libraries automatically

---

## Lexical Token Detection

### How Tokens Are Recognized

The lexer uses **regular expressions** to define patterns for each token type. When scanning input, Flex:

1. Reads characters from the input stream
2. Tries to match the longest possible string against all patterns
3. When a match is found, executes the associated action
4. Continues from where it left off

### Identifiers Detection

**Pattern Definition:**

```c
IDENTIFIER   [a-zA-Z][a-zA-Z0-9_]*
```

**Explanation:**

- `[a-zA-Z]` - Must start with a letter (uppercase or lowercase)
- `[a-zA-Z0-9_]*` - Followed by zero or more letters, digits, or underscores

**Regular Expression Breakdown:**

- `[...]` - Character class (matches any single character in the set)
- `*` - Kleene star (zero or more occurrences)

**Matching Rule:**

```flex
{IDENTIFIER}    { int stCode = addToST(yytext); addToPIF(yytext, stCode); }
```

**Examples of Valid Identifiers:**

```
x
threshold
sales_data
max_value
myVariable123
_private    (NOT valid - must start with letter)
123abc      (NOT valid - cannot start with digit)
```

**How It Works:**

1. Flex scans and recognizes the identifier pattern
2. `yytext` contains the matched identifier string
3. `addToST(yytext)` adds it to Symbol Table (or returns existing index)
4. `addToPIF(yytext, stCode)` adds token and ST reference to PIF

### Constants Detection

#### Numeric Constants

**Pattern Definition:**

```c
NUMBER   [0-9]+
```

**Explanation:**

- `[0-9]` - Matches any digit
- `+` - One or more occurrences (at least one digit required)

**Matching Rule:**

```flex
{NUMBER}    { int stCode = addToST(yytext); addToPIF(yytext, stCode); }
```

**Examples:**

```
0      ✓ Valid
10     ✓ Valid
200    ✓ Valid
1234   ✓ Valid
007    ✓ Valid (leading zeros allowed)
```

**How It Works:**

1. Scans consecutive digits
2. Converts matched text to string representation
3. Stores in Symbol Table with unique index
4. Same number appearing multiple times reuses the same ST index

#### String Constants

**Pattern Definition:**

```c
STRING   \"[a-zA-Z0-9_ ./]*\"
```

**Explanation:**

- `\"` - Literal quote character (escaped)
- `[a-zA-Z0-9_ ./]*` - Zero or more allowed characters
- `\"` - Closing quote

**Allowed characters inside strings:**

- Letters (a-z, A-Z)
- Digits (0-9)
- Underscore (\_)
- Space ( )
- Dot (.)
- Forward slash (/)

**Matching Rule:**

```flex
{STRING}    { int stCode = addToST(yytext); addToPIF(yytext, stCode); }
```

**Examples:**

```
"data.csv"              ✓ Valid
"sales_data.csv"        ✓ Valid
"output file.txt"       ✓ Valid
"path/to/file.csv"      ✓ Valid
"Hello123"              ✓ Valid
"test@email.com"        ✗ Invalid (@ not allowed)
```

### Keywords vs Identifiers

**Important:** Keywords are matched **before** identifiers using the **longest match** and **first match priority** rules.

```flex
VAR          { addToPIF(yytext, -1); }    // Keyword (matches first)
{IDENTIFIER} { ... }                       // Identifier (matches later)
```

**Example:**

- Input: `VAR` → Recognized as keyword (not identifier)
- Input: `VARIABLE` → Recognized as identifier (doesn't match VAR exactly)

---

## Data Structures for Symbol Table Management

### Symbol Table (ST) Structure - Hash Table Implementation

**Purpose:** Store identifiers, numbers, and string constants with unique indices using an efficient hash table.

**Data Structure Definition:**

```c
#define HASH_TABLE_SIZE 101  // Prime number for better distribution

/* Symbol Table node structure (for chaining) */
typedef struct STNode {
    char token[257];         // Token string (max 256 chars + null terminator)
    int index;               // Unique index in symbol table
    struct STNode* next;     // Pointer to next node (for collision chaining)
} STNode;

/* Hash Table for Symbol Table */
STNode* HashTable[HASH_TABLE_SIZE];  // Array of pointers to linked lists
int stLength = 0;                     // Total number of unique symbols
```

**Properties:**

- **Hash Table with Chaining** - Uses separate chaining for collision resolution
- **Prime-sized table** - 101 buckets for better hash distribution
- **Sequential indexing** - Indices assigned incrementally (0, 1, 2, ...)
- **No duplicates** - Same token always gets same index
- **Dynamic memory** - Nodes allocated dynamically using malloc

**Why Hash Table?**

- **O(1) average lookup time** vs O(n) for linear array
- **Efficient for large programs** with many identifiers
- **Better scalability** as program size grows
- **Industry standard** approach for symbol tables

**Hash Function (djb2 algorithm):**

```c
unsigned int hash(char* str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash % HASH_TABLE_SIZE;
}
```

**Collision Resolution:**

When two tokens hash to the same bucket, they are stored in a **linked list** (chaining method):

```
Bucket[39]: multiplier(15) -> 10(5)
Bucket[63]: 0(21) -> max_value(2)
```

This shows `multiplier` and `10` both hashed to bucket 39, so they're chained together.

### Program Internal Form (PIF) Structure

**Purpose:** Store the sequence of tokens as they appear in the program.

**Data Structure Definition:**

```c
typedef struct {
    char token[257];         // Token string
    int symbolTableCode;     // ST index or -1 for keywords/operators
} PIF;

PIF ProgramInternalForm[300];   // Array of 300 PIF entries
int pifLength = 0;              // Current number of entries
```

**Properties:**

- **Fixed-size array** - Can hold up to 300 tokens
- **Sequential storage** - Tokens stored in order of appearance
- **Symbol Table references:**
  - `-1` for keywords and operators (fixed tokens)
  - `ST index` for identifiers, numbers, and strings

---

## Symbol Table Operations

### 0. Initialize Hash Table (`initHashTable`)

**Function:**

```c
void initHashTable()
```

**Purpose:** Initialize all hash table buckets to NULL.

**Algorithm:**

```
FOR each bucket in HashTable:
    Set bucket to NULL
```

**Implementation:**

```c
void initHashTable() {
    for(int i = 0; i < HASH_TABLE_SIZE; i++) {
        HashTable[i] = NULL;
    }
}
```

**Time Complexity:** O(HASH_TABLE_SIZE) = O(1) constant

**Usage:** Called once at program start before any token processing.

### 1. Add to Symbol Table (`addToST`) - Hash Table Version

**Function:**

```c
int addToST(char* token)
```

**Purpose:** Add a token to the Symbol Table hash table or return existing index.

**Algorithm:**

```
1. Compute hash index for token
2. Search the chain at HashTable[hash_index]:
   - FOR each node in chain:
     IF token matches node's token:
       RETURN existing index
3. IF no match found:
   - Allocate new node
   - Copy token to new node
   - Assign index = current length
   - Insert at beginning of chain (most efficient)
   - Increment stLength
   - RETURN new index
```

**Implementation:**

```c
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
```

**Time Complexity:**

- **Average case:** O(1) - direct hash lookup
- **Worst case:** O(k) where k = chain length at that bucket
- Much better than O(n) for linear search!

**Example:**

```c
addToST("x")     // Computes hash, returns 0 (new entry at bucket X)
addToST("10")    // Computes hash, returns 1 (new entry at bucket Y)
addToST("x")     // Computes hash, returns 0 (existing entry found)
addToST("y")     // Computes hash, returns 2 (new entry at bucket Z)
```

### 2. Add to Program Internal Form (`addToPIF`)

**Function:**

```c
void addToPIF(char* token, int stCode)
```

**Purpose:** Record a token in the PIF with its Symbol Table reference.

**Algorithm:**

```
1. Copy token to next PIF entry
2. Store Symbol Table code (index or -1)
3. Increment pifLength
```

**Implementation:**

```c
void addToPIF(char* token, int stCode) {
    strcpy(ProgramInternalForm[pifLength].token, token);
    ProgramInternalForm[pifLength].symbolTableCode = stCode;
    pifLength++;
}
```

**Parameters:**

- `token` - The token string (keyword, identifier, operator, etc.)
- `stCode` - Symbol Table index or `-1` for keywords/operators

**Time Complexity:** O(1)

**Example Usage:**

```c
addToPIF("VAR", -1);              // Keyword
addToPIF("x", 0);                 // Identifier at ST[0]
addToPIF("=", -1);                // Operator
addToPIF("10", 1);                // Number at ST[1]
addToPIF(";", -1);                // Delimiter
```

### 3. Display Symbol Table (`showSymbolTable`) - Hash Table Version

**Function:**

```c
void showSymbolTable()
```

**Purpose:** Print all entries in the Symbol Table hash table, showing buckets and chains.

**Output Format:**

```
~~~~~~~ Symbol table (Hash Table) ~~~~~~~
Hash Table Size: 101, Total Symbols: 7
-------------------------------------------
Bucket[34]: x(0)
Bucket[39]: multiplier(15) -> 10(5)
Bucket[63]: 0(21) -> max_value(2)
...
~~~~~~~ End ST ~~~~~~~
```

**Implementation:**

```c
void showSymbolTable() {
    printf("~~~~~~~ Symbol table (Hash Table) ~~~~~~~\n");
    printf("Hash Table Size: %d, Total Symbols: %d\n", HASH_TABLE_SIZE, stLength);
    printf("-------------------------------------------\n");

    for(int i = 0; i < HASH_TABLE_SIZE; i++) {
        STNode* current = HashTable[i];
        if(current != NULL) {
            printf("Bucket[%d]: ", i);
            while(current != NULL) {
                printf("%s(%d)", current->token, current->index);
                if(current->next != NULL) {
                    printf(" -> ");  // Show chaining with arrow
                }
                current = current->next;
            }
            printf("\n");
        }
    }

    printf("~~~~~~~ End ST ~~~~~~~\n");
    printf("\n");
}
```

**Key Features:**

- Shows **bucket index** where each token is stored
- Displays **chains** with `->` arrow notation
- Reveals **collision patterns** and hash distribution
- Useful for understanding hash table performance

### 4. Display Program Internal Form (`showProgramInternalForm`)

**Function:**

```c
void showProgramInternalForm()
```

**Purpose:** Print all entries in the PIF.

**Output Format:**

```
~~~~~~~ Program internal form ~~~~~~~
 token  stCode
 token  stCode
 ...
~~~~~~~ End PIF ~~~~~~~
```

**Implementation:**

```c
void showProgramInternalForm() {
    printf("~~~~~~~ Program internal form ~~~~~~~\n");

    for(int i = 0; i < pifLength; i++) {
        printf(" %s  %d ", ProgramInternalForm[i].token,
               ProgramInternalForm[i].symbolTableCode);
        printf("\n");
    }

    printf("~~~~~~~ End PIF ~~~~~~~\n");
    printf("\n");
}
```

### 5. Free Hash Table (`freeHashTable`)

**Function:**

```c
void freeHashTable()
```

**Purpose:** Free all dynamically allocated memory in the hash table to prevent memory leaks.

**Algorithm:**

```
FOR each bucket in HashTable:
    current = bucket
    WHILE current is not NULL:
        temp = current
        current = current->next
        free(temp)
    Set bucket to NULL
```

**Implementation:**

```c
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
```

**Time Complexity:** O(n) where n = total number of nodes

**Usage:** Called once at program end after all processing is complete.

**Important:** Essential for proper memory management! Without this, the program would leak memory equal to all symbol table entries.

---

## Complete Token Processing Flow

**Step-by-step example for:** `VAR x = 10;`

1. **Input:** `VAR`

   - Pattern matched: Keyword `VAR`
   - Action: `addToPIF("VAR", -1)`
   - PIF: `[("VAR", -1)]`

2. **Input:** `x`

   - Pattern matched: `IDENTIFIER`
   - Action:
     - `stCode = addToST("x")` → Returns 0 (new entry)
     - `addToPIF("x", 0)`
   - ST: `[("x", 0)]`
   - PIF: `[("VAR", -1), ("x", 0)]`

3. **Input:** `=`

   - Pattern matched: Operator `\=`
   - Action: `addToPIF("=", -1)`
   - PIF: `[("VAR", -1), ("x", 0), ("=", -1)]`

4. **Input:** `10`

   - Pattern matched: `NUMBER`
   - Action:
     - `stCode = addToST("10")` → Returns 1 (new entry)
     - `addToPIF("10", 1)`
   - ST: `[("x", 0), ("10", 1)]`
   - PIF: `[("VAR", -1), ("x", 0), ("=", -1), ("10", 1)]`

5. **Input:** `;`
   - Pattern matched: Delimiter `\;`
   - Action: `addToPIF(";", -1)`
   - PIF: `[("VAR", -1), ("x", 0), ("=", -1), ("10", 1), (";", -1)]`

**Final State:**

- **Symbol Table:** `x` (index 0), `10` (index 1)
- **PIF:** Complete token sequence with references

---

## Hash Table vs Linear Array: Performance Comparison

### Linear Array Approach (Old)

**Structure:**

```c
ST SymbolTable[100];
```

**Search Operation:**

```c
for(int i = 0; i < stLength; i++) {
    if(strcmp(SymbolTable[i].token, token) == 0)
        return i;
}
```

**Performance:**

- ❌ O(n) lookup time
- ❌ Slow for large programs
- ❌ Every lookup scans from beginning
- ✅ Simple implementation
- ✅ No memory management needed

### Hash Table Approach (Current)

**Structure:**

```c
STNode* HashTable[101];  // Array of linked lists
```

**Search Operation:**

```c
unsigned int index = hash(token);  // O(1)
// Search only in one bucket's chain (typically 0-3 items)
STNode* current = HashTable[index];
while(current != NULL) { ... }
```

**Performance:**

- ✅ O(1) average lookup time
- ✅ Fast even with 1000+ symbols
- ✅ Direct jump to relevant bucket
- ✅ Industry standard approach
- ⚠️ Requires malloc/free
- ⚠️ Slightly more complex

### Real-World Impact

**Example:** Program with 100 identifiers

| Operation    | Linear Array       | Hash Table                   |
| ------------ | ------------------ | ---------------------------- |
| 1st lookup   | 1 comparison       | 1 hash + ~1 comparison       |
| 50th lookup  | 50 comparisons     | 1 hash + ~1 comparison       |
| 100th lookup | 100 comparisons    | 1 hash + ~1 comparison       |
| **Average**  | **50 comparisons** | **1 hash + 1-2 comparisons** |

**Speedup:** ~25-50x faster for large programs! 🚀

### Hash Table Statistics for program2.txt

```
Hash Table Size: 101
Total Symbols: 23
Used Buckets: 21
Empty Buckets: 80
Load Factor: 0.23 (23/101)
Collisions: 2 (Buckets 39 and 63 have 2 items each)
```

**Analysis:**

- Good distribution (21 different buckets used)
- Low load factor (only 23% full)
- Minimal collisions (only 2 out of 23 symbols)
- Excellent performance characteristics

---

## Next Steps

After completing lexical analysis, you would typically:

1. Build a **parser** (syntax analyzer)
2. Create an **Abstract Syntax Tree (AST)**
3. Perform **semantic analysis**
4. Generate **intermediate code** or **target code**

---

## Resources

- [Flex Manual](https://github.com/westes/flex)
- [Lex/Flex Tutorial](https://www.geeksforgeeks.org/flex-fast-lexical-analyzer-generator/)
- Course BNF Grammar: `../lab01/bnf.txt`

---

## Author Notes

This lexer was built for the LFTC (Formal Languages and Compiler Techniques) course at Babeș-Bolyai University, Semester 5.
