#!/bin/bash

test_file="test_commands.txt"

while IFS= read -r command; do
    #echo -e "\n$command"
    echo "$command" | ../minishell
done < "$test_file"
