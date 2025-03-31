/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:04:01 by carlos-j          #+#    #+#             */
/*   Updated: 2025/03/31 14:40:17 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redirection_operator(char *str)
{
	if (!str)
		return (0);
	if (ft_strncmp(str, "<", 2) == 0)
		return (1);
	if (ft_strncmp(str, ">", 2) == 0)
		return (1);
	if (ft_strncmp(str, "<<", 3) == 0)
		return (1);
	if (ft_strncmp(str, ">>", 3) == 0)
		return (1);
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
	return (open(delimiter, O_RDONLY));
}

static int	redirect_command(char *op, char *filename, t_shell *shell)
{
	int	fd;
	int	is_output;

	is_output = (op[0] == '>');
	if (ft_strncmp(op, ">", 2) == 0)
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (ft_strncmp(op, ">>", 3) == 0)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (ft_strncmp(op, "<", 2) == 0)
		fd = open(filename, O_RDONLY);
	else if (ft_strncmp(op, "<<", 3) == 0)
		fd = handle_heredoc(filename);
	else
		return (-1);
	if (fd == -1)
	{
		perror(filename);
		shell->exit_status = 1;
		return (-1);
	}
	if (is_output)
		dup2(fd, STDOUT_FILENO);
	else
		dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	handle_redirections(char **args, t_shell *shell)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (args[i])
	{
		if (is_redirection_operator(args[i]))
		{
			if (!args[i + 1])
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n",
					STDERR_FILENO);
				shell->exit_status = 2;
				return (-1);
			}
			if (redirect_command(args[i], args[i + 1], shell) == -1)
				return (-1);
			i += 2;
		}
		else
			args[j++] = args[i++];
	}
	args[j] = NULL;
	shell->exit_status = 0;
	return (0);
}
