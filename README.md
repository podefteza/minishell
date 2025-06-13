# Minishell Project 🐚
<p><i>"As beautiful as a shell."</i></p>
A simple shell implementation from scratch using only a limited set of C functions.

## 📚 Project Description
Minishell is a minimal UNIX shell that mimics Bash behavior. It handles command parsing, builtins, redirections, pipes, environment variable expansion, and proper signal handling. The purpose is to gain a deep understanding of low-level UNIX mechanisms, such as process creation, file descriptors, and memory management.

## ⚙️ Features

### ✅ Mandatory

- Custom interactive prompt.
- Builtin command history using `readline`.
- Executing binaries via relative/absolute path and `PATH` resolution.
- Single global variable allowed (only for signal handling).
- Quote handling:
  - `'` disables all interpretation inside quotes.
  - `"` disables all interpretation except variable expansion (`$`).
- Redirections:
  - `<` input redirection
  - `>` output redirection
  - `<<` heredoc with delimiter
  - `>>` output append
- Pipes (`|`) to connect commands.
- Environment variable expansion:
  - `$VAR`, `$?`, etc.
- Proper signal handling:
  - `Ctrl-C`: interrupts current command, displays a new prompt
  - `Ctrl-D`: exits the shell
  - `Ctrl-\`: ignored
- Built-in commands:
  - `echo` (supports `-n`)
  - `cd` (relative/absolute paths)
  - `pwd`
  - `export`
  - `unset`
  - `env`
  - `exit`

## 🛠️ Build & Usage

``` bash
git clone --recurse-submodules https://github.com/podefteza/minishell
```
```bash
cd minishell/
make
./minishell
```
Alternatively, to run the shell with Valgrind and check for memory leaks (excluding known readline leaks, using the readline.supp file provided):
```bash
valgrind --suppressions=readline.supp -s --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes ./minishell
```

<p align="center"><img src="https://github.com/podefteza/minishell/blob/main/ms.png"></p>

## 🔍 Testing

To compare if **minishell** output matches **bash**, you can use `tmux` with synchronized panes:

1. Open `tmux` and split the window vertically:  
   - Press `Ctrl+B`, then `%`

2. Run **minishell** in one pane and **bash** in the other.

3. Synchronize input to both panes:  
   - Press `Ctrl+B`, then type `:setw synchronize-panes on`

Now any input will be executed simultaneously in both shells for easy comparison.


## 📝 Notes
This project follows the 42 School coding standards and includes comprehensive error handling, memory management, and modular code organization. The implementation aims to replicate core bash functionality while maintaining clean, readable code structure.

## 🤝 Contributing
Minishell is a group project made with [pesoares](https://github.com/pesoares42)' help. If you're a 42 student working on minishell, feel free to use this as reference, but make sure to understand the code and implement your own solution.

<p align="center"><img src="https://github.com/podefteza/minishell/blob/main/minishell.png"></p>
