/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:28:26 by carlos-j          #+#    #+#             */
/*   Updated: 2025/04/10 09:27:45 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_quotes(char *input)
{
	int	in_single_quote;
	int	in_double_quote;

	in_single_quote = 0;
	in_double_quote = 0;
	while (*input)
	{
		if (*input == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (*input == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		input++;
	}
	if (in_single_quote || in_double_quote)
	{
		ft_puterr("minishell", UNQ, "\n", "");
		return (1);
	}
	return (0);
}

char	*handle_quotes(char *input)
{
	char	*result;

	int i, j;
	int in_squote, in_dquote;
	int squote_count, dquote_count;
	if (!input)
		return (NULL);
	squote_count = 0;
	dquote_count = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '\'')
			squote_count++;
		else if (input[i] == '"')
			dquote_count++;
		i++;
	}
	result = malloc(ft_strlen(input) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	in_squote = 0;
	in_dquote = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !in_dquote)
		{
			in_squote = !in_squote;
			if (squote_count % 2 != 0)
				result[j++] = input[i];
		}
		else if (input[i] == '"' && !in_squote)
		{
			in_dquote = !in_dquote;
			if (dquote_count % 2 != 0)
				result[j++] = input[i];
		}
		else
			result[j++] = input[i];
		i++;
	}
	result[j] = '\0';
	free(input);
	return (result);
}
