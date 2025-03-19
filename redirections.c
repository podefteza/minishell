/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:04:01 by carlos-j          #+#    #+#             */
/*   Updated: 2025/03/19 19:35:29 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_redirections(char **args, t_shell *shell)
{
	int		fd;
	int		i;
	int		j;

	(void)shell; // change later with exit codes...
	i = 0;
	j = 0;
	while (args[i])
	{
		if (ft_strncmp(args[i], "<", 2) == 0)
		{
			fd = open(args[i + 1], O_RDONLY);
			if (fd == -1)
				return (perror(args[i + 1]), -1);
			dup2(fd, STDIN_FILENO);
			close(fd);
			i += 2;
		}
		else if (ft_strncmp(args[i], ">", 2) == 0)
		{
			fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
				return (perror(args[i + 1]), -1);
			dup2(fd, STDOUT_FILENO);
			close(fd);
			i += 2;
		}
		else if (ft_strncmp(args[i], ">>", 3) == 0)
		{
			fd = open(args[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
				return (perror(args[i + 1]), -1);
			dup2(fd, STDOUT_FILENO);
			close(fd);
			i += 2;
		}
		else if (ft_strncmp(args[i], "<<", 2) == 0)
		{
			fd = handle_heredoc(args[i + 1]);
			if (fd == -1)
				return (-1);
			dup2(fd, STDIN_FILENO);
			close(fd);
			i += 2;
		}
		else
			args[j++] = args[i++];
	}
	args[j] = NULL;
	return (0);
}


int	handle_heredoc(char *delimiter)
{
	int		fd[2];
	char	*line;

	if (pipe(fd) == -1)
		return (perror("pipe"), -1);
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


