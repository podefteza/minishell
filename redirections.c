/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:04:01 by carlos-j          #+#    #+#             */
/*   Updated: 2025/04/10 10:15:39 by carlos-j         ###   ########.fr       */
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
	return (open(delimiter, O_RDONLY));
}

int	redirect_command(char *op, char *filename, t_shell *shell)
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
		ft_putstr_fd("minishell: ", STDERR_FILENO);
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

char	*get_unexpected_redir_token(char *token)
{
	static char	single_gt[] = ">";
	static char	double_gt[] = ">>";
	static char	single_lt[] = "<";
	static char	double_lt[] = "<<";

	if (!token)
		return (NULL);
	if (ft_strncmp(token, ">>", 3) == 0 && token[2] == '\0')
		return (double_gt);
	if (ft_strncmp(token, "<<", 3) == 0 && token[2] == '\0')
		return (double_lt);
	if (ft_strncmp(token, ">", 2) == 0 && token[1] == '\0')
		return (single_gt);
	if (ft_strncmp(token, "<", 2) == 0 && token[1] == '\0')
		return (single_lt);
	if (token[0] == '>')
	{
		if (ft_strlen(token) == 3 && token[1] == '>' && token[2] == '>')
			return (single_gt);
		else if (ft_strlen(token) > 3)
			return (double_gt);
		else if (token[1] == '>')
			return (double_gt);
		return (single_gt);
	}
	if (token[0] == '<')
	{
		if (ft_strlen(token) == 3 && token[1] == '<' && token[2] == '<')
			return (single_lt);
		else if (ft_strlen(token) > 3)
			return (double_lt);
		else if (token[1] == '<')
			return (double_lt);
		return (single_lt);
	}
	return (token);
}

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

int	is_invalid_redirection(char *token)
{
	if (!token)
		return (0);
	if ((token[0] == '>' || token[0] == '<') && (token[1] == '>'
			|| token[1] == '<' || token[1] == '\0'))
	{
		if ((ft_strncmp(token, ">", 2) == 0 && token[1] == '\0')
			|| (ft_strncmp(token, ">>", 3) == 0 && token[2] == '\0')
			|| (ft_strncmp(token, "<", 2) == 0 && token[1] == '\0')
			|| (ft_strncmp(token, "<<", 3) == 0 && token[2] == '\0'))
			return (0);
		return (1);
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
			if (!args[i + 1])
			{
				ft_puterr("minishell", SNT, " `newline'", "\n");
				shell->exit_status = 2;
				return (-1);
			}
			if (is_redirection_operator(args[i + 1]))
			{
				ft_puterr("minishell", SNT, " `", get_unexpected_redir_token(args[i + 1]));
				ft_putstr_fd("'\n", STDERR_FILENO);
				shell->exit_status = 2;
				return (-1);
			}
			if (redirect_command(args[i], args[i + 1], shell) == -1)
				return (-1);
			i += 2;
		}
		else if (is_invalid_redirection(args[i]))
		{
			if (i == 0 || (i > 0 && ft_strncmp(args[i - 1], "echo", 5) != 0))
			{
				ft_puterr("minishell", SNT, " `", get_unexpected_redir_token(args[i]));
				ft_putstr_fd("'\n", STDERR_FILENO);
				shell->exit_status = 2;
				return (-1);
			}
			args[j++] = args[i++];
		}
		else
			args[j++] = args[i++];
	}
	args[j] = NULL;
	shell->exit_status = 0;
	return (0);
}
