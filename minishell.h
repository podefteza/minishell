/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:12:57 by carlos-j          #+#    #+#             */
/*   Updated: 2025/04/07 13:39:51 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <limits.h>
#include <linux/limits.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <string.h>
#include "libft/libft/libft.h"
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h> // for pid_t
#include <sys/wait.h>
#include <time.h>
// check if all libraries are needed

#define HOSTNAME_MAX 256
#define PROMPT_MAX 4416
#define MAX_VAR_NAME 100

// Define ANSI color codes
#define GREEN "\001\033[32m\002"
#define RED "\001\033[31m\002"
#define YELLOW "\001\033[33m\002"
#define BLUE "\001\033[34m\002"
#define MAGENTA "\001\033[35m\002"
#define CYAN "\001\033[36m\002"
#define WHITE "\001\033[37m\002"
#define BOLD "\001\033[1m\002"
#define RESET "\001\033[0m\002"

#define TRUE 1
#define FALSE 0

// GLOBAL VARIABLE
extern volatile sig_atomic_t	g_signal_status;

struct s_shell;

typedef struct s_builtin
{
	char						*cmd;
	int							(*func)(char **, struct s_shell *);
}								t_builtin;

typedef struct s_shell
{
	char						*user;
	char						hostname[HOSTNAME_MAX];
	char						*home;
	char						**envp;
	int							exit_status;
	pid_t						last_bg_pid;
	t_builtin					builtins[8];
}								t_shell;

typedef struct s_env
{
	char						*key;
	char						*value;
	int							exported;
	struct s_env				*next;
}								t_env;

// ../builtins/builtin_setup.c
void							builtin_setup(t_builtin *builtins);

// ../builtins/cd/builtin_cd.c
int								builtin_cd(char **args, t_shell *shell);

// ../builtins/cd/builtin_cd_utils.c
void							cd_error(t_shell *shell);
char							*expand_tilde(char *path);

// ../builtins/echo/builtin_echo.c
int								builtin_echo(char **args, t_shell *shell);

// ../builtins/echo/builtin_echo_utils.c
int								is_redirection_token(char *token);
char							*get_next_token_for_echo(char **str);
char							**handle_echo(char *modified_input,
									t_shell *shell);

// ../builtins/exit/builtin_exit.c
int								builtin_exit(char **args, t_shell *shell);

// ../builtins/exit/builtin_exit_utils.c
int								handle_exit_in_first_arg(char **args);
int								handle_exit_in_second_arg(char **args);

int								builtin_pwd(char **args, t_shell *shell);
int								builtin_export(char **args, t_shell *shell);
int								builtin_unset(char **args, t_shell *shell);
int								builtin_env(char **args, t_shell *shell);
char							**handle_echo(char *modified_input,
									t_shell *shell);
int								is_valid_identifier(const char *str);
int								find_env_var(t_shell *shell, const char *key);

char							**add_or_update_env(t_shell *shell,
									const char *key, const char *value);

// path_handler.c
char							*shorten_path(const char *cwd,
									const char *home);
char							*cmd_is_path(char *cmd, t_shell *shell);
char							*build_path(char *dir, char *cmd);
char							*get_path_from_env(t_shell *shell);
char							*search_in_path(char *path, char *cmd);

// cleanup.c
void							free_array(char **array);
void							free_shell_resources(t_shell *shell);

// command_handler.c
char							*find_command(char *cmd, t_shell *shell);
void							execute_command(char **args, t_shell *shell);

// shell_setup.c
void							build_prompt(char *prompt, t_shell *shell,
									const char *display_path);
void							get_host_name(char *hostname);
void							setup_shell(t_shell *shell, char **envp);

// expansions.c
char							*expand_dollar_sign(char **input,
									t_shell *shell, pid_t shell_pid);

// expand_variables.c
char							*get_shell_name(void);
char							*expand_variables(char *input, t_shell *shell);

// input.c
char							*handle_quotes(char *input);
void							handle_input(char *input, t_shell *shell);
int								execute_builtin(char **args, t_shell *shell);

// quotes.c
int								count_quotes(char *input);
char							*handle_quotes(char *input);

// tokenize.c
int								count_words(char *input);
char							*get_next_token(char **input_ptr);
char							**split_arguments(char *input);
int								is_redirection(char c);

// pipeline.c
int								is_pipe_outside_quotes(char *input);
char							**split_pipe(char *input, t_shell *shell);
void							execute_pipeline(char **commands,
									t_shell *shell);

// pipeline_utils.c
int								check_command_access(char **args,
									t_shell *shell);
int								setup_pipe(int pipe_fds[2]);
int								has_trailing_pipe(char *input);

// redirections.c
int								handle_redirections(char **args,
									t_shell *shell);
int								handle_heredoc(char *delimiter);

// minishell.c
void							free_args(char **args);
void							free_commands(char **commands);
