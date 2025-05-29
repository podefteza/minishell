/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:04:01 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/29 01:51:17 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	apply_redirection(char *op, int fd, int *stored_fd)
{
	int	target_fd;

	if (*stored_fd != -1)
		close(*stored_fd);
	*stored_fd = fd;
	if (op[0] == '<')
		target_fd = STDIN_FILENO;
	else
		target_fd = STDOUT_FILENO;
	if (dup2(fd, target_fd) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	return (0);
}

static int	process_redirection(char *op, char *filename, t_shell *shell,
		int *fd_pair)
{
	int	fd;

	if (!filename)
	{
		ft_puterr("minishell", SNT, " `newline'", "\n");
		shell->exit_status = 2;
		return (-1);
	}
	fd = open_redirection_file(op, filename);
	if (fd == -1)
	{
		if (!g_signal_status)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			perror(filename);
			shell->exit_status = 1;
			return (-1);
		}
		else
		{
			//g_signal_status = 0; // Reset signal status after handling error
			return (-1);
		}
	}
	if (op[0] == '<')
	{
		if (apply_redirection(op, fd, &fd_pair[0]) == -1)
			return (-1);
	}
	else if (apply_redirection(op, fd, &fd_pair[1]) == -1)
		return (-1);
	return (0);
}

static int	compact_args_array(char **args)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (args[i])
	{
		if (is_redirection_operator(args[i]) && args[i + 1])
		{
			free(args[i]);
			free(args[i + 1]);
			i += 2;
		}
		else
			args[j++] = args[i++];
	}
	args[j] = NULL;
	return (j);
}

int	handle_redirections(char **args, t_shell *shell)
{
	int	i;
	int	fd_pair[2];
	int	result;

	i = 0;
	fd_pair[0] = -1;
	fd_pair[1] = -1;
	while (args[i])
	{
		if (is_redirection_operator(args[i]))
		{
			result = process_redirection(args[i], args[i + 1], shell, fd_pair);
			if (result == -1)
				return (-1);
			i += 2;
		}
		else
			i++;
	}
	compact_args_array(args);
	if (fd_pair[0] != -1)
		close(fd_pair[0]);
	if (fd_pair[1] != -1)
		close(fd_pair[1]);
	return (0);
}
