/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:44:30 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/12 19:35:35 by carlos-j         ###   ########.fr       */
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

int	count_commands(char **commands)
{
	int	count;

	count = 0;
	while (commands[count])
		count++;
	return (count);
}

void	clean_command_args(char **commands)
{
	int		i;
	char	*cleaned_arg;

	i = 0;
	while (commands[i])
	{
		cleaned_arg = handle_quotes_for_pipe(commands[i]);
		commands[i] = cleaned_arg;
		i++;
	}
}
