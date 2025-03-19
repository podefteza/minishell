/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:44:30 by carlos-j          #+#    #+#             */
/*   Updated: 2025/03/19 18:12:58 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_pipe_outside_quotes(char *input)
{
	int	in_single_quotes;
	int	in_double_quotes;

	in_single_quotes = 0;
	in_double_quotes = 0;
	while (*input)
	{
		if (*input == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		else if (*input == '"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		else if (*input == '|' && !in_single_quotes && !in_double_quotes)
			return (1);
		input++;
	}
	return (0);
}

int	has_trailing_pipe(char *input)
{
	char	*temp;

	temp = input;
	while (*temp)
		temp++;
	temp--;
	while (temp >= input && (*temp == ' ' || *temp == '\t'))
		temp--;
	if (temp >= input && *temp == '|')
		return (1);
	return (0);
}

char	**split_pipe(char *input)
{
	if (has_trailing_pipe(input))
	{
		printf("minishell: syntax error: unexpected '|'\n");
		return (NULL);
	}
	return (ft_split(input, '|'));
}

int	setup_pipe(int pipe_fds[2])
{
	if (pipe(pipe_fds) == -1)
	{
		perror("minishell: pipe");
		return (-1);
	}
	return (0);
}

int	check_command_access(char **args, t_shell *shell)
{
	if (args && args[0] && ft_strncmp(args[0], "cat", 4) == 0 && args[1])
	{
		if (access(args[1], F_OK) == -1)
		{
			ft_putstr_fd("cat: ", STDERR_FILENO);
			ft_putstr_fd(args[1], STDERR_FILENO);
			ft_putstr_fd(": ", STDERR_FILENO);
			perror("");
			shell->exit_status = 1;
			return (0);
		}
	}
	if (args && args[0] && ft_strncmp(args[0], "export", 7) == 0)
	{
		shell->exit_status = 1;
		return (0);
	}
	return (1);
}
