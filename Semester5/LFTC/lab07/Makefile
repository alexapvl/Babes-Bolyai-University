# Makefile for Lab7 - LL(1) Parser
# LFTC (Formal Languages and Compiler Techniques)

CC = gcc
CFLAGS = -Wall -g -std=c99

TARGET = ll1_parser
SOURCES = main.c grammar.c ll1_parser.c parse_tree.c lexer.c
HEADERS = grammar.h ll1_parser.h parse_tree.h lexer.h
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: $(TARGET)
	@echo "=========================================="
	@echo "LL(1) Parser built successfully!"
	@echo ""
	@echo "Usage:"
	@echo "  ./$(TARGET) --grammar <grammar_file> --input <input_file> [options]"
	@echo ""
	@echo "Examples:"
	@echo "  ./$(TARGET) --simple --grammar grammar_simple.txt --input test_simple.txt --verbose"
	@echo "  ./$(TARGET) --grammar grammar_minidsl.txt --input ../lab2/program1.txt --verbose"
	@echo "=========================================="

# Link
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Compile
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(TARGET) $(OBJECTS)
	@echo "Cleaned."

# Test simple grammar (Requirement 1)
test-simple: $(TARGET)
	@echo ""
	@echo "╔═══════════════════════════════════════════════════════════════════╗"
	@echo "║  REQUIREMENT 1: Simple Grammar Test                               ║"
	@echo "╚═══════════════════════════════════════════════════════════════════╝"
	./$(TARGET) --simple --grammar grammar_simple.txt --input test_simple.txt --verbose

# Test MiniDSL with program1 (Requirement 2)
test-dsl1: $(TARGET)
	@echo ""
	@echo "╔═══════════════════════════════════════════════════════════════════╗"
	@echo "║  REQUIREMENT 2: MiniDSL Test (program1.txt)                       ║"
	@echo "╚═══════════════════════════════════════════════════════════════════╝"
	./$(TARGET) --grammar grammar_minidsl.txt --input ../lab2/program1.txt --verbose

# Test MiniDSL with program2 (Requirement 2)
test-dsl2: $(TARGET)
	@echo ""
	@echo "╔═══════════════════════════════════════════════════════════════════╗"
	@echo "║  REQUIREMENT 2: MiniDSL Test (program2.txt)                       ║"
	@echo "╚═══════════════════════════════════════════════════════════════════╝"
	./$(TARGET) --grammar grammar_minidsl.txt --input ../lab2/program2.txt --verbose

# Test MiniDSL with local simple program
minidsl: $(TARGET)
	@echo ""
	@echo "╔═══════════════════════════════════════════════════════════════════╗"
	@echo "║  MINIDSL: Local Test (program_minidsl.txt)                        ║"
	@echo "╚═══════════════════════════════════════════════════════════════════╝"
	./$(TARGET) --grammar grammar_minidsl.txt --input program_minidsl.txt --verbose

# Test Seminar Grammar
seminar: $(TARGET)
	@echo ""
	@echo "╔═══════════════════════════════════════════════════════════════════╗"
	@echo "║  SEMINAR: Grammar Test (S->Bb|Cd, B->aB|ε, C->cC|ε)               ║"
	@echo "╚═══════════════════════════════════════════════════════════════════╝"
	./$(TARGET) --simple --grammar grammar_seminar.txt --input test_seminar.txt --verbose

# Run all tests
test: test-simple test-dsl1

# Full test including program2
test-all: test-simple test-dsl1 test-dsl2

.PHONY: all clean test test-simple test-dsl1 test-dsl2 test-all minidsl seminar
