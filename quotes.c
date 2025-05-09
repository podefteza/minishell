/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:28:26 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/09 11:18:49 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	number_of_quotes(char *input, char quote_type)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == quote_type)
			count++;
		i++;
	}
	return (count);
}

static void	handle_single_quote(char c, int *flags, int *j, char *result)
{
	int	in_squote;
	int	squote_count;

	in_squote = flags[IN_SQUOTE];
	squote_count = flags[SQUOTE_COUNT];
	if (c == '\'' && !flags[IN_DQUOTE])
	{
		in_squote = !in_squote;
		if (squote_count % 2 != 0)
			result[(*j)++] = c;
	}
	flags[IN_SQUOTE] = in_squote;
}

static void	handle_double_quote(char c, int *flags, int *j, char *result)
{
	int	in_dquote;
	int	dquote_count;

	in_dquote = flags[IN_DQUOTE];
	dquote_count = flags[DQUOTE_COUNT];
	if (c == '"' && !flags[IN_SQUOTE])
	{
		in_dquote = !in_dquote;
		if (dquote_count % 2 != 0)
			result[(*j)++] = c;
	}
	flags[IN_DQUOTE] = in_dquote;
}

static char	*process_quotes(char *input, char *result, int squote_count,
		int dquote_count)
{
	int	i;
	int	j;
	int	flags[4];

	i = 0;
	j = 0;
	flags[IN_SQUOTE] = FALSE;
	flags[SQUOTE_COUNT] = squote_count;
	flags[IN_DQUOTE] = FALSE;
	flags[DQUOTE_COUNT] = dquote_count;
	while (input[i])
	{
		handle_single_quote(input[i], flags, &j, result);
		handle_double_quote(input[i], flags, &j, result);
		if ((input[i] != '\'' || flags[IN_DQUOTE]) && (input[i] != '"'
				|| flags[IN_SQUOTE]))
			result[j++] = input[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

char	*handle_quotes(char *input)
{
	char	*result;
	int		squote_count;
	int		dquote_count;

	if (!input)
		return (NULL);
	squote_count = number_of_quotes(input, '\'');
	dquote_count = number_of_quotes(input, '"');
	result = malloc(ft_strlen(input) + 1);
	if (!result)
		return (NULL);
	result = process_quotes(input, result, squote_count, dquote_count);
	free(input);
	return (result);
}
