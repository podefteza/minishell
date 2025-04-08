/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:44:30 by carlos-j          #+#    #+#             */
/*   Updated: 2025/04/08 11:21:36 by carlos-j         ###   ########.fr       */
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

int	has_invalid_pipe_syntax(char *input)
{
	char	*trimmed;

	trimmed = input;
	while (*trimmed == ' ' || *trimmed == '\t')
		trimmed++;
	if (*trimmed == '|')
		return (1);
	while (*input)
	{
		if (*input == '<' && *(input + 1) == '|')
			return (1);
		input++;
	}
	return (has_trailing_pipe(input));
}

char	**split_pipe(char *input, t_shell *shell)
{
	char	**commands;
	int		i;
	char	*trimmed;

	if (has_trailing_pipe(input) || has_invalid_pipe_syntax(input)
		|| input[0] == '|')
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		shell->exit_status = 2;
		return (NULL);
	}
	commands = ft_split(input, '|');
	if (!commands)
		return (NULL);
	i = 0;
	while (commands[i])
	{
		trimmed = ft_strtrim(commands[i], " \t");
		free(commands[i]);
		commands[i] = trimmed;
		i++;
	}
	return (commands);
}
