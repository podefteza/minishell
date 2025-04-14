/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:12:57 by carlos-j          #+#    #+#             */
/*   Updated: 2025/04/14 21:13:58 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft/libft.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <linux/limits.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
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

#define CNF ": command not found"
#define IAD ": Is a directory"
#define NFD ": No such file or directory"
#define PND ": Permission denied"
#define SNT ": syntax error near unexpected token"
#define UNQ ": syntax error: unclosed quotes"

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
	char						**export_list;
	int							exit_status;
	int							is_prompting;
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

// ../builtins/env/builtin_env.c
int								builtin_env(char **args, t_shell *shell);
int								find_env_var(t_shell *shell, const char *key);
char							**add_or_update_env(t_shell *shell,
									const char *key, const char *value);

// ../builtins/exit/builtin_exit.c
int								builtin_exit(char **args, t_shell *shell);

// ../builtins/exit/builtin_exit_utils.c
int								handle_exit_in_first_arg(char **args,
									t_shell *shell);
int								handle_exit_in_second_arg(char **args,
									t_shell *shell);

// ../builtins/export/builtin_export.c
int								builtin_export(char **args, t_shell *shell);
int								is_valid_identifier(const char *str);

// ../builtins/export/builtin_pwd.c
int								builtin_pwd(char **args, t_shell *shell);

// ../builtins/export/unset/builtin_unset.c
int								builtin_unset(char **args, t_shell *shell);

// ../cleanup.c
void							free_array(char **array);
void							free_shell_resources(t_shell *shell);

// ../command_handler.c
void							command_not_found(char **args, t_shell *shell);
char							*find_command(char *cmd, t_shell *shell);
void							is_directory(char *full_path, t_shell *shell);
void							execute_process(char *full_path, char **args,
									int is_background, t_shell *shell);
void							execute_command(char **args, t_shell *shell);

// ../error.c
void							ft_puterr(char *msg1, char *msg2, char *msg3,
									char *msg4);

// ../expand_variables.c
char							*get_shell_name(void);
void							expand_handle_quotes(char **input, char **ptr,
									int *in_single, int *in_double);
void							expand_process_input(char **input, char **ptr,
									t_shell *shell, int *in_single_quote);
char							*expand_variables(char *input, t_shell *shell);
char							*check_for_expansion(char *final_input,
									t_shell *shell);

// ../expansions.c
char							*expand_last_bg_pid(t_shell *shell);
char							*expand_env_variable(char **input,
									t_shell *shell);
char							*expand_dollar_sign(char **input,
									t_shell *shell, pid_t shell_pid);

// ../input.c
int								is_echo_command(char *cmd);
int								execute_builtin(char **args, t_shell *shell);
char							*trim_spaces(const char *input);
char							*trim_quotes(const char *input);
void							handle_signal_status(t_shell *shell);
char							*input_with_expansion(char *final_input,
									t_shell *shell);
int								input_with_echo(char *final_input,
									char ***args_ptr, t_shell *shell);
int								input_with_pipe(char *final_input,
									t_shell *shell);
void							handle_input(char *input, t_shell *shell);

// path_handler.c
char							*shorten_path(const char *cwd,
									const char *home);
char							*cmd_is_path(char *cmd, t_shell *shell);
char							*build_path(char *dir, char *cmd);
char							*get_path_from_env(t_shell *shell);
char							*search_in_path(char *path, char *cmd);

// pipeline_utils.c
int								is_pipe_outside_quotes(char *input);
int								has_trailing_pipe(char *input);
int								has_invalid_pipe_syntax(char *input);
char							**split_pipe(char *input, t_shell *shell);

// pipeline.c
int								count_commands(char **commands);
char							**copy_args(char **args);
int								safe_execute_command(char **args,
									t_shell *shell);
int								setup_io_backups(int *original_stdin,
									int *original_stdout);
int								create_pipe_if_needed(char *next_command,
									int pipe_fds[2]);
void							close_fds(int original_stdin,
									int original_stdout, int pipe_fds[2],
									char **args);
int								setup_child_io(int input_fd, int pipe_fds[2]);
int								error_return(char *message, int ret_value);
void							child_process_work(char **args, int input_fd,
									int pipe_fds[2], t_shell *shell);
void							parent_process_work(int *input_fd,
									int pipe_fds[2], int original_stdin,
									int original_stdout);
int								process_command(char **commands, int i,
									int *input_fd, t_shell *shell);
void							clean_command_args(char **commands);
int								process_commands_in_pipeline(char **commands,
									int *input_fd, pid_t *pids, t_shell *shell);
void							wait_for_commands_and_set_status(pid_t *pids,
									int pid_count, t_shell *shell);
void							execute_pipeline(char **commands,
									t_shell *shell);

// quotes.c
int								count_quotes(char *input);
char							*handle_quotes(char *input);

// redirections.c
int								handle_heredoc(char *delimiter);
int								redirect_command(char *op, char *filename,
									t_shell *shell);
char							*get_unexpected_redir_token(char *token);
int								is_redirection_operator(char *str);
int								is_invalid_redirection(char *token);
int								handle_redirections(char **args,
									t_shell *shell);

// shell_setup.c
void							build_prompt(char *prompt, t_shell *shell,
									const char *display_path);
void							get_host_name(char *hostname);
void							user_hostname(t_shell *shell);
void							setup_shell(t_shell *shell, char **envp);

// tokenize.c
int								count_words(char *input);
char							*get_next_token(char **input_ptr);
char							**split_arguments(char *input);
