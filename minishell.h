/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:12:57 by carlos-j          #+#    #+#             */
/*   Updated: 2025/03/14 11:27:07 by carlos-j         ###   ########.fr       */
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

typedef struct s_shell
{
	char			*user;
	char			hostname[HOSTNAME_MAX];
	char			*home;
	char			**envp;
	int				exit_status;
	pid_t			last_bg_pid;
}					t_shell;

typedef struct s_builtin
{
	char			*cmd;
	void			(*func)(char **args, t_shell *shell);
}					t_builtin;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				exported;
	struct s_env	*next;
}					t_env;

// builtins
void				builtin_setup(t_builtin *builtins);
void				builtin_cd(char **args, t_shell *shell);
void				builtin_pwd(char **args, t_shell *shell);
void				builtin_echo(char **args, t_shell *shell);
char				**handle_echo(char *modified_input);
void				builtin_exit(char **args, t_shell *shell);
int					is_valid_identifier(const char *str);
int					find_env_var(t_shell *shell, const char *key);
void				builtin_export(char **args, t_shell *shell);
void				builtin_unset(char **args, t_shell *shell);

// path_handler.c
char				*shorten_path(const char *cwd, const char *home);
char				*cmd_is_path(char *cmd, t_shell *shell);
char				*build_path(char *dir, char *cmd);
char				*get_path_from_env(t_shell *shell);
char				*search_in_path(char *path, char *cmd);

// cleanup.c
void				free_args(char **args);
void				free_commands(char **commands);

// command_handler.c
char				*find_command(char *cmd, t_shell *shell);
void				execute_command(char **args, t_shell *shell);

// fork_processes.c
int					fork_processes(int *pipe_fds, char **commands,
						t_shell *shell);

// shell_setup.c
void				build_prompt(char *prompt, t_shell *shell,
						const char *display_path);
void				get_host_name(char *hostname);
void				setup_shell(t_shell *shell, char **envp);

// expand_variables.c
char				*get_env_value(char *var, t_shell *shell);
char				*expand_variables(char *input, t_shell *shell);

// format_input.c
char				*handle_quotes(char *input);
void				handle_input(char *input, t_shell *shell);

// minishell.c
void				free_args(char **args);
void				free_commands(char **commands);
