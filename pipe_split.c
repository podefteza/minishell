/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:58:40 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/09 16:27:33 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	has_trailing_pipe(char *input)
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

static int	has_invalid_pipe_syntax(char *input)
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

	if (has_trailing_pipe(input) || has_invalid_pipe_syntax(input)
		|| input[0] == '|')
	{
		ft_puterr("minishell", SNT, " `|'", "\n");
		shell->exit_status = 2;
		return (NULL);
	}
	commands = ft_split(input, '|');
	if (!commands)
		return (NULL);
	return (commands);
}
