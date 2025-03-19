/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:28:26 by carlos-j          #+#    #+#             */
/*   Updated: 2025/03/19 14:33:55 by carlos-j         ###   ########.fr       */
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
		printf("minishell: syntax error: unclosed quotes\n");
		return (1);
	}
	return (0);
}

char	*handle_quotes(char *input)
{
	int		i;
	int		j;
	int		in_single_quote;
	int		in_double_quote;
	char	*output;

	i = 0;
	j = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	output = malloc(ft_strlen(input) + 1);
	if (!output)
		return (NULL);
	while (input[i])
	{
		if (input[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (input[i] == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else
			output[j++] = input[i];
		i++;
	}
	output[j] = '\0';
	return (output);
}
