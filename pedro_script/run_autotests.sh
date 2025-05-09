#!/bin/bash

#set -x
test_file="test_commands.txt"

while IFS= read -r command; do
    echo "Executing command: $command"
    echo "$command" | valgrind -q --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --suppressions=readline_supression ../minishell # all kinds of leaks
    #echo "$command" | ./minishell # no valgrind
done < "$test_file"
