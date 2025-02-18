/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:12:57 by carlos-j          #+#    #+#             */
/*   Updated: 2025/02/18 14:16:52 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <errno.h>
#include <limits.h>
//#include <string.h>
#include <time.h>
#include <sys/wait.h>
#include <linux/limits.h>
#include "libft/libft/libft.h"
// check if all libraries are needed

#define HOSTNAME_MAX 256
#define PATH_MAX 4096
#define PROMPT_MAX 4416

// Define ANSI color codes
#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLD    "\033[1m"
#define RESET   "\033[0m"

// built_ins.c
void	builtin_cd(char **args);
void	builtin_pwd(void);
void	builtin_echo(char **args);

// path_handler.c
char	*shorten_path(const char *cwd, const char *home);
char	*cmd_is_path(char *cmd);
char	*build_path(char *dir, char *cmd);
char	*get_path_from_env(char **envp);
char	*search_in_path(char *path, char *cmd);

// command_handler.c
char	*find_command(char *cmd, char **envp);
void	execute_command(char **args, char **envp);
