#!/bin/bash

# Collect all .txt files into one command stream
command_stream=$(find cmds/ -type f -name '*.txt' -exec cat {} +)

# Optional: save combined input to a temp file if you want to review
# echo "$command_stream" > all_commands.txt

# Run once under valgrind
echo "$command_stream" | valgrind -q --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --suppressions=readline_supression ../minishell

# no valgrind
# echo "$command_stream" | ../minishell
