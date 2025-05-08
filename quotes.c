/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:28:26 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/08 13:14:26 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*strip_escaped_characters_single(char *arg)
{
	int		in_squote;
	char	*result;
	int		i;
	int		in_dquote;
	int		j;

	in_squote = 0;
	in_dquote = 0;
	i = 0;
	if (!arg)
		return (NULL);
	in_squote = 0;
	in_dquote = 0;
	result = malloc(strlen(arg) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (arg[i])
	{
		if (arg[i] == '\'' && !in_dquote)
		{
			in_squote = !in_squote;
			i++;
			continue ;
		}
		if (arg[i] == '"' && !in_squote)
		{
			in_dquote = !in_dquote;
			i++;
			continue ;
		}
		if (arg[i] == '\\' && !in_squote)
		{
			if (in_dquote)
			{
				if (strchr("$`\"\\\n", arg[i + 1]))
					i++;
			}
			else
				if (arg[i + 1])
					i++;
		}
		if (arg[i])
			result[j++] = arg[i++];
	}
	result[j] = '\0';
	free(arg);
	return (result);
}

int	count_unquoted_chars(const char *str, char c)
{
	int	count;
	int	in_single_quote;
	int	in_double_quote;

	count = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	for (int i = 0; str[i]; i++)
	{
		if (str[i] == '\'' && !in_double_quote && (i == 0 || str[i
			- 1] != '\\'))
			in_single_quote = !in_single_quote;
		else if (str[i] == '"' && !in_single_quote && (i == 0 || str[i
				- 1] != '\\'))
			in_double_quote = !in_double_quote;
		else if (str[i] == c && !in_single_quote && !in_double_quote)
			count++;
	}
	return (count);
}

char	*handle_quotes(char *arg)
{
	int		squotes;
	char	*cleaned;

	squotes = count_unquoted_chars(arg, '\'');
	if (squotes == 2 && arg[0] == '\'' && arg[strlen(arg) - 1] == '\'')
	{
		cleaned = strndup(arg + 1, strlen(arg) - 2);
		free(arg);
		return (cleaned);
	}
	if (squotes == 0 || squotes % 2 == 0)
		return (strip_escaped_characters_single(arg));
	return (arg);
}
