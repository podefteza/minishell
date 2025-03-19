/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:29:25 by carlos-j          #+#    #+#             */
/*   Updated: 2025/03/19 19:35:36 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_words(char *input)
{
	int	count;
	int	in_single_quote;
	int	in_double_quote;

	count = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (*input)
	{
		while (*input == ' ' && !in_single_quote && !in_double_quote)
			input++;
		if (!*input)
			break ;
		count++;
		while (*input)
		{
			if (*input == '\'' && !in_double_quote)
				in_single_quote = !in_single_quote;
			else if (*input == '\"' && !in_single_quote)
				in_double_quote = !in_double_quote;
			else if (*input == ' ' && !in_single_quote && !in_double_quote)
				break ;
			input++;
		}
	}
	return (count);
}

char	*get_next_token(char *input)
{
	static char	*str;
	char		*token;
	int			in_squote;
	int			in_dquote;
	int			i;

	in_squote = 0;
	in_dquote = 0;
	i = 0;
	if (input)
		str = input;
	if (!str || *str == '\0')
		return (NULL);
	token = malloc(ft_strlen(str) + 1);
	if (!token)
		return (NULL);
	while (*str && (*str == ' ' || *str == '\t'))
		str++;
	while (*str)
	{
		if (*str == '\'' && !in_dquote)
			in_squote = !in_squote;
		else if (*str == '\"' && !in_squote)
			in_dquote = !in_dquote;
		else if (*str == ' ' && !in_squote && !in_dquote)
			break ;
		else
			token[i++] = *str;
		str++;
	}
	token[i] = '\0';
	while (*str == ' ')
		str++;
	if (!*str)
		str = NULL;
	return (token);
}

char	**split_arguments(char *input)
{
	int		arg_count;
	char	**args;
	char	*token;
	int		i;

	arg_count = count_words(input);
	args = malloc(sizeof(char *) * (arg_count + 1));
	if (!args)
		return (NULL);
	i = 0;
	token = get_next_token(input);
	while (token)
	{
		args[i++] = ft_strdup(token);
		free(token);
		token = get_next_token(NULL);
	}
	args[i] = NULL;
	return (args);
}
