#!/bin/bash

# Check if hyperfine is installed
if ! command -v hyperfine &> /dev/null
then
    echo "hyperfine could not be found, please install it first."
    exit
fi

# Parse options
CALC_COMPILE=true
while [[ "$#" -gt 0 ]]; do
    case $1 in
        --ignorecompile) CALC_COMPILE=false; shift ;;
        *) echo "Unknown parameter passed: $1"; exit 1 ;;
    esac
    shift
done

# Create or clear the stats.txt file
echo "" > stats.txt

# Loop through all directories
for dir in */; do
    # Enter the directory
    cd "$dir" || continue

    # Find all cpp files
    for file in *.cpp; do
        # Compile the cpp file
        compile_command="g++ -o ${file%.cpp}.out $file"
        echo "Compiling $file in $dir"

        if $CALC_COMPILE; then
            # Measure compile time
            hyperfine --warmup 20 --export-markdown compile_time.md "$compile_command"
            cat compile_time.md >> ../stats.txt
            rm compile_time.md
        else
            # Just compile without measuring time
            $compile_command
        fi

        # Measure execution time
        if [ -f "${file%.cpp}.out" ]; then
            echo "Running ${file%.cpp}.out in $dir"
            hyperfine --warmup 30 --export-markdown execution_time.md "./${file%.cpp}.out"
            cat execution_time.md >> ../stats.txt
            rm execution_time.md
        fi
    done

    # Return to the parent directory
    cd ..
done

echo "Benchmarking completed. Results are in stats.txt"
