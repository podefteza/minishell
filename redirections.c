/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:04:01 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/05 15:06:17 by carlos-j         ###   ########.fr       */
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

int	redirect_command(int apply_redirection, char *op, char *filename,
		t_shell *shell)
{
	int	fd;
	int	is_output;

	is_output = (op[0] == '>');
	fd = open_redirection_file(op, filename);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(filename);
		shell->exit_status = 1;
		return (-1);
	}
	if (apply_redirection)
	{
		if (is_output)
			dup2(fd, STDOUT_FILENO);
		else
			dup2(fd, STDIN_FILENO);
	}
	close(fd);
	return (0);
}

int	process_valid_redirection(char **args, int i, t_shell *shell)
{
	int	apply_redirection;

	if (!args[i + 1])
	{
		ft_puterr("minishell", SNT, " `newline'", "\n");
		shell->exit_status = 2;
		return (-1);
	}
	if (is_redirection_operator(args[i + 1]))
	{
		print_redirection_syntax_error(shell, args[i + 1]);
		return (-1);
	}
	apply_redirection = (i > 0);
	if (redirect_command(apply_redirection, args[i], args[i + 1], shell) == -1)
		return (-1);
	return (0);
}

int	handle_invalid_redirection_token(char **args, int i, t_shell *shell)
{
	if (i == 0 || (i > 0 && ft_strncmp(args[i - 1], "echo", 5) != 0))
	{
		print_redirection_syntax_error(shell, args[i]);
		return (-1);
	}
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
			if (process_valid_redirection(args, i, shell) == -1)
				return (-1);
			i += 2;
		}
		else if (is_invalid_redirection(args[i]))
		{
			if (handle_invalid_redirection_token(args, i, shell) == -1)
				return (-1);
			args[j++] = args[i++];
		}
		else
			args[j++] = args[i++];
	}
	args[j] = NULL;
	shell->exit_status = 0;
	return (0);
}
