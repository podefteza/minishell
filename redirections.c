/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:04:01 by carlos-j          #+#    #+#             */
/*   Updated: 2025/03/21 16:05:49 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	return (open(delimiter, O_RDONLY)); // Temporary placeholder.
}

int	handle_redirections(char **args, t_shell *shell)
{
	int		fd;
	int		i;
	int		j;
	char	*file;

	i = 0;
	j = 0;
	while (args[i])
	{
		if (args[i][0] == '<' || args[i][0] == '>')
		{
			if (args[i][0] == '>' && args[i][1] != '>' && args[i][1] != '\0')
			{
				file = ft_strdup(args[i] + 1);
				fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (fd == -1)
				{
					perror(file);
					free(file);
					shell->exit_status = 1;
					return (-1);
				}
				dup2(fd, STDOUT_FILENO);
				close(fd);
				free(file);
				i++;
			}
			else if (args[i][0] == '>' && args[i][1] == '>'
				&& args[i][2] != '\0')
			{
				file = ft_strdup(args[i] + 2);
				fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
				if (fd == -1)
				{
					perror(file);
					free(file);
					shell->exit_status = 1;
					return (-1);
				}
				dup2(fd, STDOUT_FILENO);
				close(fd);
				free(file);
				i++;
			}
			else if (args[i][0] == '<' && args[i][1] != '<'
				&& args[i][1] != '\0')
			{
				file = ft_strdup(args[i] + 1);
				fd = open(file, O_RDONLY);
				if (fd == -1)
				{
					perror(file);
					free(file);
					shell->exit_status = 1;
					return (-1);
				}
				dup2(fd, STDIN_FILENO);
				close(fd);
				free(file);
				i++;
			}
			else if (args[i][0] == '<' && args[i][1] == '<'
				&& args[i][2] != '\0')
			{
				file = ft_strdup(args[i] + 2);
				fd = handle_heredoc(file);
				if (fd == -1)
				{
					free(file);
					shell->exit_status = 1;
					return (-1);
				}
				dup2(fd, STDIN_FILENO);
				close(fd);
				free(file);
				i++;
			}
			else
			{
				if (!args[i + 1])
				{
					perror("Missing filename/delimiter for redirection");
						// or syntax error
					shell->exit_status = 2;
					return (-1);
				}
				file = args[i + 1];
				if (ft_strncmp(args[i], ">", 2) == 0)
					fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				else if (ft_strncmp(args[i], ">>", 3) == 0)
					fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
				else if (ft_strncmp(args[i], "<", 2) == 0)
					fd = open(file, O_RDONLY);
				else if (ft_strncmp(args[i], "<<", 3) == 0)
					fd = handle_heredoc(file);
				if (fd == -1)
				{
					perror(file);
					shell->exit_status = 1;
					return (-1);
				}
				if (args[i][0] == '>')
					dup2(fd, STDOUT_FILENO);
				else
					dup2(fd, STDIN_FILENO);
				close(fd);
				i += 2;
			}
		}
		else
			args[j++] = args[i++];
	}
	args[j] = NULL;
	shell->exit_status = 0;
	return (0);
}
