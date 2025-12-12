# Lab 7: LL(1) Parser Implementation

## Overview

This lab implements **syntactical analysis** using the **LL(1) parsing algorithm** for the miniDSL language. The parser outputs the parse tree in **father-sibling table representation** for maximum grade (10).

---

## Table of Contents

1. [What is LL(1) Parsing?](#what-is-ll1-parsing)
2. [How the Algorithm Works](#how-the-algorithm-works)
3. [FIRST and FOLLOW Sets](#first-and-follow-sets)
4. [The Parsing Table](#the-parsing-table)
5. [The Parsing Algorithm](#the-parsing-algorithm)
6. [Father-Sibling Table](#father-sibling-table)
7. [Files](#files)
8. [Usage](#usage)

---

## What is LL(1) Parsing?

**LL(1)** is a **top-down, predictive parsing algorithm**. The name means:

| Letter | Meaning |
|--------|---------|
| **L** | Reads input **L**eft to right |
| **L** | Produces **L**eftmost derivation |
| **(1)** | Uses **1** token of lookahead to make parsing decisions |

### Comparison with Other Parsers

| Parser Type | Direction | Method | Example Tools |
|-------------|-----------|--------|---------------|
| **LL(1)** | Top-down | Predictive | Hand-written, ANTLR |
| **LR(0)/LALR** | Bottom-up | Shift-reduce | Yacc, Bison |
| **Recursive Descent** | Top-down | Function calls | Hand-written |

---

## How the Algorithm Works

### Overview

```
┌─────────────────────────────────────────────────────────────────────┐
│                        LL(1) PARSER PIPELINE                         │
├─────────────────────────────────────────────────────────────────────┤
│                                                                       │
│  ┌─────────────┐     ┌─────────────┐     ┌─────────────────────────┐ │
│  │   Grammar   │ ──► │ Compute     │ ──► │ Build Parsing Table     │ │
│  │   Rules     │     │ FIRST/FOLLOW│     │ M[NonTerminal,Terminal] │ │
│  └─────────────┘     └─────────────┘     └─────────────────────────┘ │
│                                                    │                  │
│                                                    ▼                  │
│  ┌─────────────┐     ┌─────────────┐     ┌─────────────────────────┐ │
│  │   Input     │ ──► │   Lexer     │ ──► │  Predictive Parser      │ │
│  │   Program   │     │   (Tokens)  │     │  (Stack + Table)        │ │
│  └─────────────┘     └─────────────┘     └─────────────────────────┘ │
│                                                    │                  │
│                                                    ▼                  │
│                                          ┌─────────────────────────┐ │
│                                          │   Parse Tree            │ │
│                                          │   (Father-Sibling)      │ │
│                                          └─────────────────────────┘ │
└─────────────────────────────────────────────────────────────────────┘
```

### Step-by-Step Process

1. **Load the grammar** from a file
2. **Compute FIRST sets** for all symbols
3. **Compute FOLLOW sets** for all non-terminals
4. **Build the parsing table** using FIRST and FOLLOW
5. **Parse the input** using a stack and the table
6. **Build the parse tree** as a father-sibling table

---

## FIRST and FOLLOW Sets

### FIRST Set

**FIRST(α)** = the set of terminals that can appear as the **first symbol** of any string derived from α.

#### Rules for Computing FIRST:

```
1. If X is a terminal:
   FIRST(X) = {X}

2. If X is ε:
   FIRST(ε) = {ε}

3. If X is a non-terminal with production X → Y₁Y₂...Yₖ:
   - Add FIRST(Y₁) \ {ε} to FIRST(X)
   - If ε ∈ FIRST(Y₁):
     - Add FIRST(Y₂) \ {ε} to FIRST(X)
     - If ε ∈ FIRST(Y₂): continue to Y₃, etc.
   - If ε ∈ FIRST(Yᵢ) for all i: add ε to FIRST(X)
```

#### Example:

For grammar:
```
E  → T E'
E' → + T E' | ε
T  → F T'
T' → * F T' | ε
F  → ( E ) | id
```

FIRST sets:
```
FIRST(F)  = { (, id }
FIRST(T') = { *, ε }
FIRST(T)  = { (, id }
FIRST(E') = { +, ε }
FIRST(E)  = { (, id }
```

### FOLLOW Set

**FOLLOW(A)** = the set of terminals that can appear **immediately after** A in some derivation.

#### Rules for Computing FOLLOW:

```
1. Add $ to FOLLOW(S) where S is the start symbol

2. For each production A → αBβ:
   - Add FIRST(β) \ {ε} to FOLLOW(B)

3. For each production A → αB or A → αBβ where ε ∈ FIRST(β):
   - Add FOLLOW(A) to FOLLOW(B)
```

#### Example (continued):

```
FOLLOW(E)  = { ), $ }
FOLLOW(E') = { ), $ }
FOLLOW(T)  = { +, ), $ }
FOLLOW(T') = { +, ), $ }
FOLLOW(F)  = { *, +, ), $ }
```

---

## The Parsing Table

The parsing table **M[A, a]** tells us which production to use when:
- The top of the stack is non-terminal **A**
- The current input token is terminal **a**

### Building the Table:

```
For each production A → α:

1. For each terminal a in FIRST(α):
   - Add "A → α" to M[A, a]

2. If ε ∈ FIRST(α):
   - For each terminal b in FOLLOW(A):
     - Add "A → α" to M[A, b]
```

### Example Table:

|       | id  | +   | *   | (   | )   | $   |
|-------|-----|-----|-----|-----|-----|-----|
| E     | E→TE'|     |     |E→TE'|     |     |
| E'    |     |E'→+TE'|   |     |E'→ε |E'→ε |
| T     |T→FT'|     |     |T→FT'|     |     |
| T'    |     |T'→ε |T'→*FT'|   |T'→ε |T'→ε |
| F     |F→id |     |     |F→(E)|     |     |

### LL(1) Conflicts

If a cell has **more than one production**, the grammar is **not LL(1)**. Common causes:
- **Left recursion**: `A → Aα` (must be eliminated)
- **Left factoring needed**: `A → αβ₁ | αβ₂` (must factor out α)
- **Ambiguity**: Multiple derivations for same string

---

## The Parsing Algorithm

```
Initialize:
  - Push $ onto stack
  - Push start symbol S onto stack
  - Read first input token

While stack is not empty:
  Let X = top of stack
  Let a = current input token
  
  If X is a terminal or $:
    If X == a:
      Pop X from stack
      Read next input token
    Else:
      ERROR
  
  Else (X is a non-terminal):
    If M[X, a] contains production X → Y₁Y₂...Yₖ:
      Pop X from stack
      Push Yₖ, Yₖ₋₁, ..., Y₁ onto stack (reverse order)
      OUTPUT production number
    Else:
      ERROR

If input is completely consumed:
  ACCEPT
Else:
  ERROR
```

### Parsing Example

Parsing `id + id * id` with the expression grammar:

| Step | Stack | Input | Action |
|------|-------|-------|--------|
| 1 | $E | id+id*id$ | E → TE' |
| 2 | $E'T | id+id*id$ | T → FT' |
| 3 | $E'T'F | id+id*id$ | F → id |
| 4 | $E'T'id | id+id*id$ | match id |
| 5 | $E'T' | +id*id$ | T' → ε |
| 6 | $E' | +id*id$ | E' → +TE' |
| 7 | $E'T+ | +id*id$ | match + |
| 8 | $E'T | id*id$ | T → FT' |
| ... | ... | ... | ... |
| n | $ | $ | ACCEPT |

---

## Father-Sibling Table

The **father-sibling representation** is a memory-efficient way to store the parse tree.

### Structure

Each node stores:
- **Index**: Unique node identifier (1-based)
- **Info**: Symbol name (terminal or non-terminal)
- **Father**: Index of parent node (0 = root)
- **Right Sibling**: Index of next sibling (0 = none)

### Example

For the derivation of `a b`:
```
       S
      / \
     a   A
        / \
       b   A
           |
           ε
```

Father-Sibling Table:
```
┌─────────┬──────────┬────────────┬─────────────────┐
│  Index  │   Info   │   Father   │  Right Sibling  │
├─────────┼──────────┼────────────┼─────────────────┤
│       1 │    S     │          0 │               0 │
│       2 │    a     │          1 │               3 │
│       3 │    A     │          1 │               0 │
│       4 │    b     │          3 │               5 │
│       5 │    A     │          3 │               0 │
│       6 │    ε     │          5 │               0 │
└─────────┴──────────┴────────────┴─────────────────┘
```

### Advantages

1. **Memory efficient**: Only 2 pointers per node
2. **Easy traversal**: Can find all children of a node
3. **Compact output**: Easy to store and transmit

---

## Files

| File | Description |
|------|-------------|
| `grammar.h/c` | Grammar data structures and loading |
| `ll1_parser.h/c` | FIRST/FOLLOW computation, table building, parsing |
| `parse_tree.h/c` | Father-sibling tree representation |
| `lexer.h/c` | Lexical analyzer (adapted from lab06) |
| `main.c` | Driver program |
| `grammar_simple.txt` | Simple test grammar (Requirement 1) |
| `grammar_minidsl.txt` | MiniDSL grammar (Requirement 2) |
| `test_simple.txt` | Test input for simple grammar |
| `Makefile` | Build system |

---

## Usage

### Building

```bash
make
```

### Testing Simple Grammar (Requirement 1)

```bash
make test-simple
```

Or manually:
```bash
./ll1_parser --simple --grammar grammar_simple.txt --input test_simple.txt --verbose
```

### Testing MiniDSL (Requirement 2)

```bash
make test-dsl1   # Test with program1.txt
make test-dsl2   # Test with program2.txt
make test-all    # Run all tests
```

Or manually:
```bash
./ll1_parser --grammar grammar_minidsl.txt --input ../lab02/program1.txt --verbose
```

### Output

The parser outputs:
1. **Grammar definition** (terminals, non-terminals, productions)
2. **FIRST sets** for all non-terminals
3. **FOLLOW sets** for all non-terminals
4. **Parsing table** M[A, a]
5. **Parsing steps** (if --verbose)
6. **Parse tree** in father-sibling table format
7. **Visual tree** representation

---

## References

- Aho, Lam, Sethi, Ullman: *Compilers: Principles, Techniques, and Tools* (Dragon Book)
- Lab 01: BNF Grammar Definition
- Lab 06: Lexer with FA Validation

---

## Author

Lab 7 - LFTC (Formal Languages and Compiler Techniques)
