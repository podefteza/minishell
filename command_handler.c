/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 14:15:03 by carlos-j          #+#    #+#             */
/*   Updated: 2025/02/19 13:21:39 by carlos-j         ###   ########.fr       */
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

void	execute_command(char **args, char **envp)
{
	pid_t	pid;
	char	*full_path;

	if (!args || !args[0])
		return ;
	full_path = find_command(args[0], envp);
	if (!full_path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd("\n", 2);
		return ;
	}
	pid = fork();
	if (pid < 0)
		perror("fork");
	else if (pid == 0)
	{
		if (execve(full_path, args, envp) == -1)
			perror("execve");
		exit(EXIT_FAILURE);
	}
	else
		waitpid(pid, NULL, 0);
	if (full_path != args[0])
		free(full_path);
}
