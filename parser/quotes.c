/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:28:26 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/30 14:17:26 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

static void	handle_single_quote(char c, t_quote_flags *flags, int *j,
		char *result)
{
	if (c == '\'' && !flags->in_double_quote)
	{
		flags->in_single_quote = !flags->in_single_quote;
		if (flags->single_quote_count % 2 != 0)
			result[(*j)++] = c;
	}
}

static void	handle_double_quote(char c, t_quote_flags *flags, int *j,
		char *result)
{
	if (c == '"' && !flags->in_single_quote)
	{
		flags->in_double_quote = !flags->in_double_quote;
		if (flags->double_quote_count % 2 != 0)
			result[(*j)++] = c;
	}
}

static char	*process_quotes(char *input, char *result, int single_quote_count,
		int double_quote_count)
{
	int				i;
	int				j;
	t_quote_flags	flags;

	i = 0;
	j = 0;
	flags.in_single_quote = 0;
	flags.single_quote_count = single_quote_count;
	flags.in_double_quote = 0;
	flags.double_quote_count = double_quote_count;
	while (input[i])
	{
		handle_single_quote(input[i], &flags, &j, result);
		handle_double_quote(input[i], &flags, &j, result);
		if ((input[i] != '\'' || flags.in_double_quote) && (input[i] != '"'
				|| flags.in_single_quote))
			result[j++] = input[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

char	*handle_quotes(char *input)
{
	char	*result;
	int		single_quote_count;
	int		double_quote_count;

	if (!input)
		return (NULL);
	single_quote_count = number_of_quotes(input, '\'');
	double_quote_count = number_of_quotes(input, '"');
	result = malloc(ft_strlen(input) + 1);
	if (!result)
		return (NULL);
	result = process_quotes(input, result, single_quote_count,
			double_quote_count);
	return (result);
}
