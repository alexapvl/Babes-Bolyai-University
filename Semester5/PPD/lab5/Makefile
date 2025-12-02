# Makefile for Polynomial and Big Number Multiplication Lab

# Java compiler
JAVAC = javac
JAVA = java

# Source files
SOURCES = Polynomial.java \
          RegularMultiplication.java \
          KaratsubaMultiplication.java \
          PerformanceBenchmark.java \
          PolynomialTest.java \
          BigNumber.java \
          BigNumberMultiplication.java \
          BigNumberBenchmark.java \
          Main.java

# Default target
.PHONY: all
all: compile

# Compile all Java files
.PHONY: compile
compile:
	@echo "Compiling Java files..."
	$(JAVAC) $(SOURCES)
	@echo "Compilation successful!"

# Run tests
.PHONY: test
test: compile
	@echo "Running tests..."
	$(JAVA) PolynomialTest

# Run demo
.PHONY: demo
demo: compile
	@echo "Running demo..."
	$(JAVA) Main demo

# Run interactive menu
.PHONY: run
run: compile
	@echo "Starting interactive menu..."
	$(JAVA) Main

# Quick polynomial benchmark (degree 1000)
.PHONY: benchmark-quick
benchmark-quick: compile
	@echo "Running quick polynomial benchmark..."
	$(JAVA) PerformanceBenchmark 1000

# Comprehensive polynomial benchmark
.PHONY: benchmark-poly
benchmark-poly: compile
	@echo "Running comprehensive polynomial benchmark..."
	$(JAVA) PerformanceBenchmark

# Comprehensive big number benchmark
.PHONY: benchmark-bignum
benchmark-bignum: compile
	@echo "Running comprehensive big number benchmark..."
	$(JAVA) BigNumberBenchmark

# Clean compiled files
.PHONY: clean
clean:
	@echo "Cleaning compiled files..."
	rm -f *.class
	@echo "Clean complete!"

# Help target
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  make              - Compile all Java files (default)"
	@echo "  make compile      - Compile all Java files"
	@echo "  make test         - Run polynomial tests"
	@echo "  make demo         - Run demonstration"
	@echo "  make run          - Run interactive menu"
	@echo "  make benchmark-quick  - Run quick polynomial benchmark (degree 1000)"
	@echo "  make benchmark-poly   - Run comprehensive polynomial benchmark"
	@echo "  make benchmark-bignum - Run comprehensive big number benchmark"
	@echo "  make clean        - Remove compiled .class files"
	@echo "  make help         - Show this help message"
	@echo ""
	@echo "Examples:"
	@echo "  make test"
	@echo "  make demo"
	@echo "  make benchmark-quick"

