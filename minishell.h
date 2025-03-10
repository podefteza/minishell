/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:12:57 by carlos-j          #+#    #+#             */
/*   Updated: 2025/03/10 15:45:04 by carlos-j         ###   ########.fr       */
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
//#define PATH_MAX 4096
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

typedef struct s_builtin
{
	char	*cmd;
	void	(*func)(char **args, int *exit_status);
}			t_builtin;

typedef struct s_shell
{
	char	*user;
	char	hostname[HOSTNAME_MAX];
	char	*home;
}			t_shell;

// builtins
void		builtin_setup(t_builtin *builtins);
void		builtin_cd(char **args, int *exit_status);
void		builtin_pwd(char **args, int *exit_status);
void		builtin_echo(char **args, int *exit_status);
char		**handle_echo(char *modified_input);
void		builtin_exit(char **args, int *exit_status);

// path_handler.c
char		*shorten_path(const char *cwd, const char *home);
char		*cmd_is_path(char *cmd, int *exit_status);
char		*build_path(char *dir, char *cmd);
char		*get_path_from_env(char **envp);
char		*search_in_path(char *path, char *cmd);

// cleanup.c
void		free_args(char **args);
void		free_commands(char **commands);

// command_handler.c
char		*find_command(char *cmd, char **envp, int *exit_status);
void		execute_command(char **args, char **envp, int *exit_status);

// fork_processes.c
void		handle_pipe(char *input, char **envp);
int			fork_processes(int *pipe_fds, char **commands, char **envp,
				int exit_status);

// shell_setup.c
void		build_prompt(char *prompt, const char *user, const char *hostname,
				const char *display_path);
void		get_host_name(char *hostname);
void		setup_shell(t_shell *shell);

// expand_variables.c
char		*get_env_value(char *var, char **envp);
char		*expand_variables(char *input, char **envp, int *exit_status);

// format_input.c
char		*handle_quotes(char *input);
void		handle_input(char *input, char **envp, int *exit_status);

// get_pid.c
pid_t		last_background_pid(pid_t pid);

// minishell.c
void		free_args(char **args);
void		free_commands(char **commands);
