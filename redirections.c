/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:04:01 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/19 11:51:35 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_heredoc(char *delimiter)
{
	int		fd[2];
	char	*line;

	if (pipe(fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
			break ;
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	free(line);
	close(fd[1]);
	return (fd[0]);
}

int	handle_redirections(char **args, t_shell *shell)
{
	int		i;
	int		j;
	int		input_fd;
	int		output_fd;
	char	*op;
	char	*filename;
	int		fd;

	i = 0;
	j = 0;
	input_fd = -1;
	output_fd = -1;
	while (args[i])
	{
		if (is_redirection_operator(args[i]))
		{
			op = args[i];
			filename = args[i + 1];
			if (!filename)
			{
				ft_puterr("minishell", SNT, " `newline'", "\n");
				shell->exit_status = 2;
				return (-1);
			}
			fd = open_redirection_file(op, filename);
			if (fd == -1)
			{
				ft_putstr_fd("minishell: ", STDERR_FILENO);
				perror(filename);
				shell->exit_status = 1;
				return (-1);
			}
			if (op[0] == '<')
			{
				if (input_fd != -1)
					close(input_fd);
				input_fd = fd;
				if (dup2(fd, STDIN_FILENO) == -1)
				{
					perror("dup2");
					close(fd);
					return (-1);
				}
			}
			else
			{
				if (output_fd != -1)
					close(output_fd);
				output_fd = fd;
				if (dup2(fd, STDOUT_FILENO) == -1)
				{
					perror("dup2");
					close(fd);
					return (-1);
				}
			}
			i += 2;
		}
		else
		{
			args[j++] = args[i++];
		}
	}
	args[j] = NULL;
	if (input_fd != -1)
		close(input_fd);
	if (output_fd != -1)
		close(output_fd);
	return (0);
}
