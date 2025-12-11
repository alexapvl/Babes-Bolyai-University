# Makefile for Lab6 - Parser with Lab3 Lexer

CC = gcc
CFLAGS = -Wall -g -DLAB6_PARSER_MODE -Wno-unused-but-set-variable -Wno-implicit-function-declaration
YACC = bison

TARGET = parser

all: $(TARGET)

# Compile lexer.c (copied from lexer.l) directly as C source with parser
$(TARGET): parser.tab.c parser.tab.h lexer.c
	$(CC) $(CFLAGS) -o $(TARGET) parser.tab.c lexer.c
	@echo "=========================================="
	@echo "Parser built successfully!"
	@echo "Usage: ./$(TARGET) <input_file>"
	@echo "=========================================="

parser.tab.c parser.tab.h: parser.y
	$(YACC) -d parser.y

lexer.c: lexer.l
	cp lexer.l lexer.c

clean:
	rm -f $(TARGET) parser.tab.c parser.tab.h lexer.c
	@echo "Cleaned generated files."

test: $(TARGET)
	@echo "\n=========================================="
	@echo "Testing program1.txt..."
	@echo "=========================================="
	@./$(TARGET) ../lab2/program1.txt
	@echo "\n=========================================="
	@echo "Testing program2.txt..."
	@echo "=========================================="
	@./$(TARGET) ../lab2/program2.txt

.PHONY: all clean test

