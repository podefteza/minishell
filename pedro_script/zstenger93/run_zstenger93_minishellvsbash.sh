#!/bin/bash

# Directory containing test files
test_dir="cmds"

# Temporary files
bash_out=$(mktemp)
minishell_out=$(mktemp)

# Cleanup function
cleanup() {
    rm -f "$bash_out" "$minishell_out"
}
trap cleanup EXIT

# Function to print separator line
print_separator() {
    echo -e "\n\033[1;36m------------------------------------------------\033[0m\n"
}

find "$test_dir" -type f -name "*.txt" | while read -r test_file; do
    echo -e "\n\033[1;35m==============================================\033[0m"
    echo -e "\033[1;35mRunning tests from: $test_file\033[0m"
    echo -e "\033[1;35m==============================================\033[0m"
    
    while IFS= read -r command; do
        [[ -z "$command" || "$command" =~ ^# ]] && continue
        
        print_separator
        echo -e "\033[1;34mCommand: \033[1;37m$command\033[0m"
        
        # Execute in bash
        echo -e "\033[1;32m=== BASH OUTPUT ===\033[0m"
        echo "$command" | bash &> "$bash_out"
        cat "$bash_out"
        
        # Execute in minishell
        echo -e "\n\033[1;33m=== MINISHELL OUTPUT ===\033[0m"
        echo "$command" | valgrind -q --leak-check=full --show-leak-kinds=all \
            --track-origins=yes --track-fds=yes \
            --suppressions=readline_supression \
            ../../minishell &> "$minishell_out"
        cat "$minishell_out"
        
    done < "$test_file"
done

print_separator
echo -e "\033[1;32mAll tests completed\033[0m"
print_separator
