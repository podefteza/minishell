/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:12:57 by carlos-j          #+#    #+#             */
/*   Updated: 2025/02/28 18:54:36 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <errno.h>
#include <limits.h>
#include <linux/limits.h>
//#include <string.h>
#include <time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h> // for pid_t
#include "libft/libft/libft.h"
// check if all libraries are needed

#define HOSTNAME_MAX 256
//#define PATH_MAX 4096
#define PROMPT_MAX 4416
#define MAX_VAR_NAME 100

// Define ANSI color codes
#define GREEN   "\001\033[32m\002"
#define RED     "\001\033[31m\002"
#define YELLOW  "\001\033[33m\002"
#define BLUE    "\001\033[34m\002"
#define MAGENTA "\001\033[35m\002"
#define CYAN    "\001\033[36m\002"
#define WHITE   "\001\033[37m\002"
#define BOLD    "\001\033[1m\002"
#define RESET   "\001\033[0m\002"

#define TRUE 1
#define FALSE 0

// global variable to store the last exit code
extern int	g_exit_status;

typedef struct s_builtin
{
	char	*cmd;
	void	(*func)(char **args);
}	t_builtin;

// builtins
void	builtin_setup(t_builtin *builtins);
void	builtin_cd(char **args);
void	builtin_pwd(char **args);
void	builtin_echo(char **args);
char **handle_echo(char *modified_input);
void	builtin_exit(char **args);

// path_handler.c
char	*shorten_path(const char *cwd, const char *home);
char	*cmd_is_path(char *cmd);
char	*build_path(char *dir, char *cmd);
char	*get_path_from_env(char **envp);
char	*search_in_path(char *path, char *cmd);

// command_handler.c
char	*find_command(char *cmd, char **envp);
void	execute_command(char **args, char **envp);

// fork_processes.c
void	handle_pipe(char *input, char **envp);
int	fork_processes(int *pipe_fds, char **commands, char **envp);

// shell_setup.c
void	build_prompt(char *prompt, const char *user, const char *hostname,
	const char *display_path);
void	get_host_name(char *hostname);

// expand_variables.c
char	*get_env_value(char *var, char **envp);
char	*expand_variables(char *input, char **envp);

// exit_status.c
char	*last_exit_status(char *input);

// format_input.c
char	*handle_quotes(char *input);
void handle_input(char *input, char **envp);

// get_pid.c
pid_t	last_background_pid(pid_t pid);


// minishell.c
void	free_args(char **args);
void	free_commands(char **commands);
