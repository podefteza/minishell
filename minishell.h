/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:12:57 by carlos-j          #+#    #+#             */
/*   Updated: 2025/06/18 11:42:59 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft/libft.h"
#include <errno.h>
#include <fcntl.h>
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
#include <unistd.h>

#define GREEN "\001\033[32m\002"
#define BLUE "\001\033[34m\002"
#define BOLD "\001\033[1m\002"
#define RESET "\001\033[0m\002"
#define HOSTNAME_MAX 256
#define PROMPT_MAX 4096
#define TRUE 1
#define FALSE 0
#define CNF ": command not found"
#define IAD ": Is a directory"
#define NFD ": No such file or directory"
#define PND ": Permission denied"
#define SNT ": syntax error near unexpected token"
#define UNQ ": syntax error: unclosed quotes"

extern int			g_signal_status;

struct				s_shell;

typedef struct s_quote_flags
{
	int				in_single_quote;
	int				single_quote_count;
	int				in_double_quote;
	int				double_quote_count;
}					t_quote_flags;

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
	t_list			*temp_files;
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
	t_shell			*shell;
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

typedef struct s_heredoc_info
{
	char			*delimiter;
	int				delim_len;
	int				quoted;
	char			*delim_end;
}					t_heredoc_info;

// ./
// ./builtins/
// ./builtins/cd/
// ./builtins/cd/builtin_cd_utils_2.c
int					is_tilde_expansion_point(char *input, int i);
int					get_username_length(char *input, int start);
int					is_current_user(char *input, int start, int len,
						t_shell *shell);
void				update_quote_state(char c, int *in_single, int *in_double);
int					copy_home(char *result, int j, char *home);
// ./builtins/cd/builtin_cd_utils.c
char				*expand_tilde_unquoted(char *input, t_shell *shell);
void				handle_home_directory(t_shell *shell, char *home,
						char *old_pwd);
// ./builtins/cd/builtin_cd.c
int					builtin_cd(char **args, t_shell *shell);
// ./builtins/cd/builtin_oldpwd.c
void				change_to_previous_directory(t_shell *shell,
						int should_print);
// ./builtins/echo/
// ./builtins/echo/builtin_echo_utils.c
int					skip_n_flags(char **args, int *newline);
int					is_redirection_token(char *token);
int					is_quoted_redirection(char *token);
// ./builtins/echo/builtin_echo.c
int					builtin_echo(char **args, t_shell *shell);
// ./builtins/env/
// ./builtins/env/builtin_env_utils.c
int					find_env_var(t_shell *shell, const char *key);
char				**add_or_update_env(t_shell *shell, const char *key,
						const char *value);
// ./builtins/env/builtin_env.c
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
// ./builtins/export/builtin_export_utils3.c
void				handle_simple_export(char *arg, t_shell *shell, char *copy);
void				handle_append_export(char *arg, t_shell *shell, char *copy);
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
// ./execution/child_process_utils.c
void				exit_if_invalid_command(char **args, t_shell *shell);
// ./execution/child_process.c
void				execute_child(t_shell *shell, char **args, int prev,
						int pipe_fd[2]);
// ./execution/command_execution.c
void				handle_command(t_shell *shell, pid_t *child_pids,
						t_exec_state *state);
// ./execution/command_handler_utils.c
char				*find_command(char *cmd, t_shell *shell);
int					count_executables(char ***commands);
void				safe_close(int fd);
// ./execution/command_handler.c
int					execute_command(t_shell *shell);
// ./execution/execute_final_command.c
void				execute_final_command(t_shell *shell);
// ./execution/finalize_execution.c
int					finalize_execution(t_shell *shell, pid_t *child_pids,
						t_exec_state *state);
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
char				*find_env_value(char *var_name, t_shell *shell);
char				*expand_dollar_sign(char **input, t_shell *shell);

// ./parser/
// ./parser/heredoc_content_utils.c
int					process_single_heredoc_line(char **content, char *delimiter,
						int expand, t_shell *shell);
// ./parser/heredoc_content.c
char				*collect_heredoc_content(char *delimiter, int expand,
						t_shell *shell);
// ./parser/heredoc_delimiter.c
t_heredoc_info		*parse_heredoc_delimiter(char *heredoc_start);
// ./parser/heredoc_replacement.c
int					handle_heredoc_replacement(char **result, char **pos,
						t_shell *shell);
// ./parser/heredoc_temp_file.c
char				*handle_heredoc_to_file(char *delimiter, int expand,
						t_shell *shell);
// ./parser/input_utils.c
int					process_initial_input(t_shell *shell);
int					remove_quotes_from_commands(t_shell *shell);
// ./parser/input.c
void				handle_input(t_shell *shell);
// ./parser/process_heredoc.c
char				*preprocess_heredocs(char *input, t_shell *shell);
// ./parser/quotes_utils.c
int					count_quotes(char *input);
int					is_quoted(char *str);
char				*remove_quotes_concat(const char *str);
int					handle_quote_error(char ***cmds, int i, int j,
						int is_empty);
// ./parser/tokenize_utils.c
int					count_args(char **args);
char				**list_to_array(t_list *lst);
char				*get_next_token_tokenizer(t_tokenizer *t);
// ./parser/tokenize.c
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
int					handle_redirections(char **args, t_shell *shell);

// ./shell/
// ./shell/shell_loop.c
void				run_shell_loop(t_shell *shell);
// ./shell/shell_setup_utils.c
void				user_hostname(t_shell *shell);
void				update_shlvl(t_shell *shell);
void				build_prompt(char *prompt, t_shell *shell,
						const char *display_path);
// ./shell/shell_setup.c
void				setup_shell(t_shell *shell, char **envp);

// ./utils/
// ./utils/cleanup.c
void				free_array(char **array);
void				free_commands_array(char ***commands);
void				free_input(t_shell *shell);
void				free_shell_resources(t_shell *shell);
// ./utils/error.c
void				ft_puterr(char *msg1, char *msg2, char *msg3, char *msg4);
// ./utils/general_utils.c
void				cleanup_all_temp_files(t_shell *shell);
int					ft_isspace(int c);
char				*ft_strndup(const char *s, size_t n);
void				restore_stdio(int out, int in);
void				close_all_fds(void);
// ./utils/signals.c
t_shell				*get_shell_context(t_shell *new_shell);
void				handle_signal(int sig, siginfo_t *info, void *context);
void				setup_signals(void);
