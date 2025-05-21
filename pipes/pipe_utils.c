/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:44:30 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/21 15:10:34 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	check_quote_state(char c, int *in_quotes, char *quote_char)
{
	if ((c == '\'' || c == '"'))
	{
		if (*in_quotes && *quote_char == c)
			*in_quotes = 0;
		else if (!*in_quotes)
		{
			*in_quotes = 1;
			*quote_char = c;
		}
	}
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

int	count_pipes(const char *s)
{
	int		count;
	int		in_quotes;
	char	quote;

	count = 0;
	in_quotes = 0;
	quote = 0;
	while (*s)
	{
		check_quote_state(*s, &in_quotes, &quote);
		if (!in_quotes && *s == '|')
			count++;
		s++;
	}
	return (count);
}

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
			return (TRUE);
		input++;
	}
	return (FALSE);
}
