# Makefile for Polynomial and Big Number Multiplication Lab
# Lab 5 (CPU) + Lab 7 (MPI)

# Java compiler
JAVAC = javac
JAVA = java

# MPJ Express settings (for MPI)
MPJ_HOME ?= $(HOME)/mpj
MPJRUN = $(MPJ_HOME)/bin/mpjrun.sh
MPJ_CLASSPATH = $(MPJ_HOME)/lib/mpj.jar

# Source files (Lab 5 - CPU implementation)
SOURCES = Polynomial.java \
          RegularMultiplication.java \
          KaratsubaMultiplication.java \
          PerformanceBenchmark.java \
          PolynomialTest.java \
          BigNumber.java \
          BigNumberMultiplication.java \
          BigNumberBenchmark.java \
          Main.java

# MPI Source files (Lab 7)
MPI_SOURCES = MPIPolynomial.java \
              MPIBigNumber.java \
              MPIBenchmark.java

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

# ============================================================================
# MPI TARGETS (Lab 7)
# ============================================================================

# Compile MPI Java files
.PHONY: compile-mpi
compile-mpi:
	@echo "Compiling MPI Java files..."
	@if [ ! -d "$(MPJ_HOME)" ]; then \
		echo "Error: MPJ_HOME not found at $(MPJ_HOME)"; \
		echo "Please set MPJ_HOME environment variable"; \
		exit 1; \
	fi
	$(JAVAC) -cp $(MPJ_CLASSPATH):. *.java
	@echo "MPI compilation successful!"

# Compile all (CPU + MPI)
.PHONY: compile-all
compile-all: compile compile-mpi
	@echo "All files compiled successfully!"

# Run MPI polynomial benchmark (4 processes)
NP ?= 4
.PHONY: mpi-poly
mpi-poly: compile-mpi
	@echo "Running MPI Polynomial benchmark with $(NP) processes..."
	MPJ_HOME=$(MPJ_HOME) $(MPJRUN) -np $(NP) MPIPolynomial

# Run MPI big number benchmark (4 processes)
.PHONY: mpi-bignum
mpi-bignum: compile-mpi
	@echo "Running MPI Big Number benchmark with $(NP) processes..."
	MPJ_HOME=$(MPJ_HOME) $(MPJRUN) -np $(NP) MPIBigNumber

# Run comprehensive MPI benchmark
.PHONY: mpi-benchmark
mpi-benchmark: compile-mpi
	@echo "Running comprehensive MPI benchmark with $(NP) processes..."
	MPJ_HOME=$(MPJ_HOME) $(MPJRUN) -np $(NP) MPIBenchmark

# Run comparison: Java CPU vs MPI
.PHONY: compare
compare: compile compile-mpi
	@echo ""
	@echo "═══════════════════════════════════════════════════════════════════════════════"
	@echo "  JAVA CPU vs MPI COMPARISON"
	@echo "═══════════════════════════════════════════════════════════════════════════════"
	@echo ""
	@echo ">>> Java CPU Benchmark (degree 1000) <<<"
	$(JAVA) PerformanceBenchmark 1000
	@echo ""
	@echo ">>> MPI Benchmark (degree 1000, $(NP) processes) <<<"
	MPJ_HOME=$(MPJ_HOME) $(MPJRUN) -np $(NP) MPIPolynomial

# Check MPJ Express installation
.PHONY: check-mpj
check-mpj:
	@echo "Checking MPJ Express installation..."
	@if [ -d "$(MPJ_HOME)" ]; then \
		echo "✓ MPJ_HOME found: $(MPJ_HOME)"; \
	else \
		echo "✗ MPJ_HOME not found: $(MPJ_HOME)"; \
		echo "  Please set MPJ_HOME environment variable"; \
		echo "  Example: export MPJ_HOME=/path/to/mpj"; \
		exit 1; \
	fi
	@if [ -f "$(MPJ_CLASSPATH)" ]; then \
		echo "✓ MPJ JAR found: $(MPJ_CLASSPATH)"; \
	else \
		echo "✗ MPJ JAR not found: $(MPJ_CLASSPATH)"; \
		exit 1; \
	fi
	@echo "MPJ Express installation looks good!"

# Help target
.PHONY: help
help:
	@echo "Polynomial & Big Number Multiplication - Lab 5 (CPU) + Lab 7 (MPI)"
	@echo ""
	@echo "═══════════════════════════════════════════════════════════════════════════════"
	@echo "  CPU TARGETS (Lab 5)"
	@echo "═══════════════════════════════════════════════════════════════════════════════"
	@echo "  make              - Compile all Java files (default)"
	@echo "  make compile      - Compile CPU Java files"
	@echo "  make test         - Run polynomial tests"
	@echo "  make demo         - Run demonstration"
	@echo "  make run          - Run interactive menu"
	@echo "  make benchmark-quick  - Run quick polynomial benchmark (degree 1000)"
	@echo "  make benchmark-poly   - Run comprehensive polynomial benchmark"
	@echo "  make benchmark-bignum - Run comprehensive big number benchmark"
	@echo ""
	@echo "═══════════════════════════════════════════════════════════════════════════════"
	@echo "  MPI TARGETS (Lab 7)"
	@echo "═══════════════════════════════════════════════════════════════════════════════"
	@echo "  make compile-mpi  - Compile MPI Java files"
	@echo "  make compile-all  - Compile both CPU and MPI files"
	@echo "  make mpi-poly     - Run MPI polynomial benchmark (NP processes)"
	@echo "  make mpi-bignum   - Run MPI big number benchmark (NP processes)"
	@echo "  make mpi-benchmark - Run comprehensive MPI benchmark"
	@echo "  make compare      - Compare Java CPU vs MPI performance"
	@echo "  make check-mpj    - Check MPJ Express installation"
	@echo ""
	@echo "═══════════════════════════════════════════════════════════════════════════════"
	@echo "  OTHER"
	@echo "═══════════════════════════════════════════════════════════════════════════════"
	@echo "  make clean        - Remove compiled .class files"
	@echo "  make help         - Show this help message"
	@echo ""
	@echo "Variables:"
	@echo "  NP=N              - Number of MPI processes (default: 4)"
	@echo "  MPJ_HOME=/path    - Path to MPJ Express installation"
	@echo ""
	@echo "Examples:"
	@echo "  make test"
	@echo "  make benchmark-poly"
	@echo "  make mpi-poly NP=8"
	@echo "  make compare NP=4"

