#!/bin/bash

test_file="test_commands2.txt"

while IFS= read -r command; do
    #echo -e "\n$command"
    echo "$command" | ../minishell
done < "$test_file"
