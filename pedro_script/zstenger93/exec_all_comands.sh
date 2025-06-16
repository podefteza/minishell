#!/bin/bash

# Directory containing test files
test_dir="cmds"

# Create a temporary file to hold all commands
cmd_file=$(mktemp)

# Find all .txt files recursively and concatenate their commands
find "$test_dir" -type f -name "*.txt" | while read -r test_file; do
    echo "==============================================" >> "$cmd_file"
    echo "# Running tests from: $test_file" >> "$cmd_file"
    echo "==============================================" >> "$cmd_file"

    # Filter out empty lines and comments, add 'echo' between commands for separation
    grep -v -E '^(#|$)' "$test_file" | sed 's/$/\necho "\n"/' >> "$cmd_file"

    echo "" >> "$cmd_file"
done

echo "All commands prepared. Starting Minishell..."
echo "exit" >> "$cmd_file"  # Ensure Minishell exits at the end

# Start Minishell once and feed all commands
valgrind -q --leak-check=full --show-leak-kinds=all \
    --track-origins=yes --track-fds=yes \
    --suppressions=readline_supression \
    ../../minishell < "$cmd_file"

# Clean up
rm -f "$cmd_file"
echo "All tests completed"
