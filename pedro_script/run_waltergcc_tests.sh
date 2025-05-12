#!/bin/bash

test_file="test_waltergcc.txt"

while IFS= read -r command; do
    echo "Executing command: $command"
    echo "$command" | valgrind -q --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --suppressions=readline_supression ../minishell
done < "$test_file"
