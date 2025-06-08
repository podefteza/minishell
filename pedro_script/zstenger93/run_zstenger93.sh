#!/bin/bash

# Directory containing test files
test_dir="cmds"

# Find all .txt files recursively
find "$test_dir" -type f -name "*.txt" | while read -r test_file; do
    echo "=============================================="
    echo "Running tests from: $test_file"
    echo "=============================================="
    
    while IFS= read -r command; do
        # Skip empty lines and comments starting with #
        [[ -z "$command" || "$command" =~ ^# ]] && continue
        
        echo "Executing command: $command"
        echo "$command" | valgrind -q --leak-check=full --show-leak-kinds=all \
            --track-origins=yes --track-fds=yes \
            --suppressions=readline_supression \
            ../../minishell
    done < "$test_file"
    
    echo ""
done

echo "All tests completed"
