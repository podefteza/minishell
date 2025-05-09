#!/bin/bash

GREEN="\001\033[1;32m\002"    # Bold Green
BLUE="\001\033[1;34m\002"     # Bold Blue
WHITE="\001\033[37m\002"      # Normal White
RESET="\001\033[0m\002"       # Reset

test_file="test_commands2.txt"
short_pwd="${PWD#"$HOME"}"

while IFS= read -r command; do
    printf "${GREEN}[bash] ${BLUE}$USER@$HOSTNAME${WHITE}:~$short_pwd${GREEN}\$${RESET} "
    printf "%s\n" "$command"
    eval "$command" 2>&1
    printf "${GREEN}[bash] ${BLUE}$USER@$HOSTNAME${WHITE}:~$short_pwd${GREEN}\$${RESET} "
    printf "exit\n"
done < "$test_file"
