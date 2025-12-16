#!/bin/bash

# Distributed Shared Memory (DSM) - Build and Run Script

# Colors for output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}════════════════════════════════════════════════════════════${NC}"
echo -e "${BLUE}    Distributed Shared Memory (DSM) - Lab 8                  ${NC}"
echo -e "${BLUE}════════════════════════════════════════════════════════════${NC}"
echo

# Get script directory
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$SCRIPT_DIR"

# Create output directory
mkdir -p out

# Compile
echo -e "${GREEN}▶ Compiling Java sources...${NC}"
javac -d out src/dsm/*.java src/main/*.java

if [ $? -ne 0 ]; then
    echo "❌ Compilation failed!"
    exit 1
fi

echo -e "${GREEN}✓ Compilation successful${NC}"
echo

# Run
echo -e "${GREEN}▶ Running DSM demonstration...${NC}"
echo
java -cp out main.MainProgram

