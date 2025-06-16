#!/bin/bash

GREEN="\001\033[1;32m\002"    # Bold Green
BLUE="\001\033[1;34m\002"     # Bold Blue
WHITE="\001\033[37m\002"      # Normal White
RESET="\001\033[0m\002"       # Reset

#test_file="cmds/mand/0_compare_parsing.txt"
#test_file="cmds/mand/1_builtins.txt"
#test_file="cmds/mand/1_pipelines.txt"
#test_file="cmds/mand/1_redirs.txt"
test_file="cmds/mand/1_scmds.txt"

short_pwd="${PWD#"$HOME"}"

# Function to display our custom prompt
show_prompt() {
    printf "${GREEN}[minishell] ${BLUE}$USER@$HOSTNAME${WHITE}:~$short_pwd${GREEN}\$${RESET} "
}

# Read and execute commands while skipping empty lines
while IFS= read -r command; do
    if [[ -n "$command" ]]; then  # Skip empty lines
        show_prompt
        printf "%s\n" "$command"  # Show the command being executed
        eval "$command" 2>&1
    fi
done < "$test_file"
