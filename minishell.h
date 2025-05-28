/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:12:57 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/28 13:34:13 by carlos-j         ###   ########.fr       */
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
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>

// check if all libraries are needed

#define HOSTNAME_MAX 256
#define PROMPT_MAX 4416
#define MAX_VAR_NAME 100
#define MAX_PIPELINE_CMDS 64

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

#define IN_SQUOTE 0
#define SQUOTE_COUNT 1
#define IN_DQUOTE 2
#define DQUOTE_COUNT 3

#define CNF ": command not found"
#define IAD ": Is a directory"
#define NFD ": No such file or directory"
#define PND ": Permission denied"
#define SNT ": syntax error near unexpected token"
#define UNQ ": syntax error: unclosed quotes"

/**
 * 	"global variable to indicate a received signal"
 * 	"0: no signal received"
 * 	"1: SIGINT received"
 * 	"2: SIGQUIT received"
**/
extern int			g_signal_status;

struct s_shell;

typedef struct s_builtin
{
	char			*cmd;
	int				(*func)(char **, struct s_shell *);
}					t_builtin;

typedef struct s_input
{
	char			*raw;
	char			*trim_spaces;
	char			*processed;
	char			*expanded;
	char			**args;
	char			***commands;
	//int				heredoc_fd;
}					t_input;

typedef struct s_shell
{
	char			*user;
	char			hostname[HOSTNAME_MAX];
	char			*home;
	char			**envp;
	char			**export_list;
	int				exit_status;
	int				is_prompting;
	int				should_exit;
	t_list *temp_files;
	pid_t			last_bg_pid;
	t_builtin		builtins[8];
	t_input			input;
}					t_shell;

typedef struct s_tokenizer
{
	const char		*input;
	int				pos;
	bool			done;
	bool			in_quotes;
	char			quote_char;
	t_shell *shell;
}					t_tokenizer;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				exported;
	struct s_env	*next;
}					t_env;

typedef struct s_exec_state
{
	int				cmd_idx;
	int				pid_idx;
	int				prev_read;
	int				pipe_fd[2];
}					t_exec_state;

// ./
// ./builtins/
// ./builtins/cd/
// ./builtins/cd/builtin_cd_utils.c
char				*expand_tilde_unquoted(char *input);
// ./builtins/cd/builtin_cd.c
int					builtin_cd(char **args, t_shell *shell);
// ./builtins/cd/builtin_oldpwd.c
void				change_to_previous_directory(t_shell *shell);
// ./builtins/echo/
// ./builtins/echo/builtin_echo_utils.c
int					count_args_for_echo(char *message);
// ./builtins/echo/builtin_echo.c
int					builtin_echo(char **args, t_shell *shell);
// ./builtins/env/
// ./builtins/env/builtin_env.c
int					find_env_var(t_shell *shell, const char *key);
char				**add_or_update_env(t_shell *shell, const char *key,
						const char *value);
int					builtin_env(char **args, t_shell *shell);
// ./builtins/exit/
// ./builtins/exit/builtin_exit_utils.c
int					handle_exit_in_first_arg(char **args, t_shell *shell);
int					handle_exit_in_second_arg(char **args, t_shell *shell);
// ./builtins/exit/builtin_exit.c
int					builtin_exit(char **args, t_shell *shell);
// ./builtins/export/
// ./builtins/export/builtin_export_utils.c
void				print_export_error(char *arg, t_shell *shell);
void				sort_env(char **envp);
// ./builtins/export/builtin_export_utils2.c
int					find_export_var(char **export_list, const char *key);
char				**add_or_update_export_list(char **export_list,
						const char *key, const char *value);
// ./builtins/export/builtin_export.c
int					builtin_export(char **args, t_shell *shell);
// ./builtins/pwd/
// ./builtins/pwd/builtin_pwd.c
int					builtin_pwd(char **args, t_shell *shell);
// ./builtins/unset/
// ./builtins/unset/unset/builtin_unset_utils.c
void				remove_env_var(t_shell *shell, char *var_name);
void				remove_export_var(t_shell *shell, char *var_name);
// ./builtins/unset/unset/builtin_unset.c
int					is_valid_identifier(const char *str);
int					builtin_unset(char **args, t_shell *shell);
// ./builtins/builtin_utils.c
void				builtin_setup(t_builtin *builtins);
int					execute_builtins(t_shell *shell, char **cmd);

// ./execution/
// ./execution/child_process.c
void				execute_child(t_shell *shell, char **args, int prev,
						int pipe_fd[2]);
// ./execution/command_execution.c
void	handle_command(t_shell *shell, pid_t *child_pids, t_exec_state *state);
int	finalize_execution(t_shell *shell, pid_t *child_pids, t_exec_state *state);
// ./execution/command_handler_utils.c
char				*find_command(char *cmd, t_shell *shell);
int					count_executables(char ***commands);
void				safe_close(int fd);
// ./execution/command_handler.c
int					execute_command(t_shell *shell);
// ./execution/path_handler.c
char				*shorten_path(const char *cwd, const char *home);
int					cmd_is_path(char *cmd, char **result, t_shell *shell);
char				*get_path_from_env(t_shell *shell);
char				*search_in_path(char *path, char *cmd);

// ./expansions/
// ./expansions/expand_variables_utils.c
void				expand_handle_quotes(char **input, char **ptr,
						int *in_single, int *in_double);
char				*get_shell_name(void);
void				check_for_expansion(t_shell *shell);
// ./expansions/expand_variables.c
char				*expand_variables(char *input, t_shell *shell);
// ./expansions/expansions.c
char				*expand_dollar_sign(char **input, t_shell *shell);

// ./parser/
// ./parser/input_utils.c
void	process_initial_input(t_shell *shell);
void				handle_signal_status(t_shell *shell);
int				remove_quotes_from_commands(t_shell *shell);
// ./parser/input.c
void close_all_fds();
void				handle_input(t_shell *shell);
// ./parser/quotes_utils.c
int					count_quotes(char *input);
int					is_quoted(char *str);
// ./parser/quotes.c
char				*handle_quotes(char *input);
// ./parser/tokenize_utils.c
char				**list_to_array(t_list *lst);
char				*get_next_token_tokenizer(t_tokenizer *t);
// ./parser/tokenize.c
char	*preprocess_heredocs(char *input, t_shell *shell);
void				split_commands(t_shell *shell);
// ./parser/validate_syntax_utils.c
void				print_syntax_error(char *token);
// ./parser/validate_syntax.c
int					validate_syntax(t_shell *shell);

// ./pipes/
// ./pipes/pipe_split.c
char				**split_pipe(t_shell *shell);
// ./pipes/pipe_utils.c
int					has_trailing_pipe(char *input);
void				check_quote_state(char c, int *in_quotes, char *quote_char);
int					has_invalid_pipe_syntax(char *input);
int					count_pipes(const char *s);
int					is_pipe_outside_quotes(char *input);

// ./redirections/
// ./redirections/redirections_utils.c
int					is_redirection_operator(char *str);
int					open_redirection_file(char *op, char *filename);
// ./redirections/redirections.c
int	handle_heredoc(char *delimiter, int expand, t_shell *shell);
int					handle_redirections(char **args, t_shell *shell);

// ./shell/
// ./shell/shell_loop.c
void				run_shell_loop(t_shell *shell);
// ./shell/shell_loop_utils.c
void				user_hostname(t_shell *shell);
// ./shell/shell_setup.c
void				build_prompt(char *prompt, t_shell *shell,
						const char *display_path);
void				setup_shell(t_shell *shell, char **envp);

// ./utils/
// ./utils/cleanup.c
void				free_array(char **array);
void free_commands_array(char ***commands);
void	free_input(t_shell *shell);
void				free_shell_resources(t_shell *shell);
// ./utils/error.c
void				ft_puterr(char *msg1, char *msg2, char *msg3, char *msg4);
// ./utils/general_utils.c
void cleanup_all_temp_files(t_shell *shell);
int					is_redirection_token(char *token);
int					ft_isspace(int c);
char				*ft_strndup(const char *s, size_t n);
void				restore_stdio(int out, int in);
// ./utils/signals.c
t_shell				*get_shell_context(t_shell *new_shell);
void				handle_signal(int sig, siginfo_t *info, void *context);
void				setup_signals(void);
