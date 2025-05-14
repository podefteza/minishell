/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_processing_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 14:44:47 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/14 10:27:53 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_io_backups(int *original_stdin, int *original_stdout)
{
	*original_stdin = dup(STDIN_FILENO);
	*original_stdout = dup(STDOUT_FILENO);
	if (*original_stdin == -1 || *original_stdout == -1)
	{
		perror("minishell: dup");
		return (-1);
	}
	return (0);
}

int	create_pipe_if_needed(char *next_command, int pipe_fds[2])
{
	if (next_command != NULL)
	{
		if (pipe(pipe_fds) == -1)
		{
			perror("minishell: pipe");
			return (-1);
		}
	}
	return (0);
}

void	close_fds(int pipe_fds[2], char **args)
{
	if (pipe_fds[0] != -1)
		close(pipe_fds[0]);
	if (pipe_fds[1] != -1)
		close(pipe_fds[1]);
	free_array(args);
}

int	error_return(char *message, int ret_value)
{
	ft_putstr_fd(message, STDERR_FILENO);
	return (ret_value);
}

int	is_empty(const char *str)
{
	while (*str)
	{
		if (*str != ' ' && *str != '\t')
			return (0);
		str++;
	}
	return (1);
}
