/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:04:01 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/12 15:50:35 by carlos-j         ###   ########.fr       */
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

static int	count_redirections(char **args)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (args[i])
	{
		if (is_redirection_operator(args[i]))
			count++;
		i++;
	}
	return (count);
}

static int	handle_if_redirection(char **args, int *i, int *j, t_shell *shell,
		int total)
{
	int redirection_result = 0;
	if (is_redirection_operator(args[*i]))
	{
		redirection_result = process_valid_redirection(args, *i, shell, total);
		if (redirection_result == -1)
		{
			//fprintf(stderr, "error\n");
			return (-1);
		}
		*i += 2;
	}
	else if (is_invalid_redirection(args[*i]))
	{
		if (handle_invalid_redirection_token(args, *i, shell) == -1)
		{
			//fprintf(stderr, "error2\n");
			return (-1);
		}
		args[(*j)++] = args[(*i)++];
	}
	else
		args[(*j)++] = args[(*i)++];
	return (0);
}

static int	finalize_redirection_args(char **args, int j, t_shell *shell)
{
	if (j == 0)
	{
		args = NULL;
		return (-1);
	}
	args[j] = NULL;
	shell->exit_status = 0;
	return (0);
}

int	handle_redirections(char **args, t_shell *shell)
{
	int	i;
	int	j;
	int	total_redirections;

	total_redirections = count_redirections(args);
	i = 0;
	j = 0;
	int redirection_result = 0;
	while (args[i])
	{
		redirection_result = handle_if_redirection(args, &i, &j, shell,
				total_redirections);
		if (redirection_result == -1)
		{
			//fprintf(stderr, "error3\n");
			return (-1);

		}
	}
	return (finalize_redirection_args(args, j, shell));
}
