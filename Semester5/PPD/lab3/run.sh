!/bin/bash

# Compile Java files
echo "Compiling Java files..."
javac MatrixMultiplication.java Main.java

if [ $? -eq 0 ]; then
    echo "Compilation successful!"
    echo ""
    echo "Running experiments..."
    echo ""
    java Main
else
    echo "Compilation failed!"
    exit 1
fi

