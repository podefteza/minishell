#!/bin/bash
#test_file="cmds/mand/0_compare_parsing.txt"
#test_file="cmds/mand/1_builtins.txt"
#test_file="cmds/mand/1_pipelines.txt"
#test_file="cmds/mand/1_redirs.txt"
test_file="cmds/mand/1_scmds.txt"


# Read non-empty lines and join them with newlines
commands=$(awk 'NF {printf "%s\\n", $0}' "$test_file")

# Feed all commands to a single minishell instance
printf "%b" "$commands" | ../minishell
