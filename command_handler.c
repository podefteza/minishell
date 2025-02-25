/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 14:15:03 by carlos-j          #+#    #+#             */
/*   Updated: 2025/02/24 13:40:37 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_command(char *cmd, char **envp)
{
	char	*path;
	char	*full_path;

	full_path = cmd_is_path(cmd);
	if (full_path)
		return (full_path);
	path = get_path_from_env(envp);
	if (!path)
		return (NULL);
	return (search_in_path(path, cmd));
}

void	command_not_found(char **args)
{
	ft_putstr_fd("minishell: command not found: ", 2);
	ft_putstr_fd(args[0], 2);
	ft_putstr_fd("\n", 2);
	g_exit_status = 127;
}

void	is_directory(char *full_path)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(full_path, 2);
	ft_putstr_fd(": Is a directory\n", 2);
	g_exit_status = 126;
	free(full_path);
}

void	execute_process(char *full_path, char **args, char **envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		perror("fork");
	else if (pid == 0)
	{
		if (execve(full_path, args, envp) == -1)
		{
			perror("execve");
			g_exit_status = 126;
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_exit_status = 128 + WTERMSIG(status);
	}
}

void	execute_command(char **args, char **envp)
{
	char		*full_path;
	struct stat	st;

	if (!args || !args[0])
		return ;
	full_path = find_command(args[0], envp);
	if (!full_path)
	{
		command_not_found(args);
		return ;
	}
	if (stat(full_path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		is_directory(full_path);
		return ;
	}
	execute_process(full_path, args, envp);
	if (full_path != args[0])
		free(full_path);
}
