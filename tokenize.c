/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:29:25 by carlos-j          #+#    #+#             */
/*   Updated: 2025/04/07 14:06:53 by carlos-j         ###   ########.fr       */
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

// change for, to while loops
char	*get_next_token(char **input_ptr)
{
	char	*str;
	char	*token;
	int		in_squote, in_dquote, i;
	int		count;

	str = *input_ptr;
	in_squote = 0;
	in_dquote = 0;
	i = 0;
	if (!str || *str == '\0')
		return (NULL);
	while (*str == ' ' || *str == '\t')
		str++;
	if (*str == '<' || *str == '>')
	{
		count = 1;
		char first_char = *str;
		while (str[count] == first_char)
			count++;

		if (count >= 3)
		{
			token = malloc(count + 1);
			if (!token)
				return (NULL);
			for (i = 0; i < count; i++)
				token[i] = *str++;
			token[i] = '\0';
		}
		else
		{
			token = malloc(3);
			if (!token)
				return (NULL);
			token[i++] = *str++;
			if ((*str == '>' && token[0] == '>') || (*str == '<' && token[0] == '<'))
				token[i++] = *str++;
			token[i] = '\0';
		}
		*input_ptr = str;
		return (token);
	}
	token = malloc(ft_strlen(str) + 1);
	if (!token)
		return (NULL);
	while (*str)
	{
		if (*str == '\'' && !in_dquote)
			in_squote = !in_squote;
		else if (*str == '\"' && !in_squote)
			in_dquote = !in_dquote;
		else if ((*str == ' ' || *str == '<' || *str == '>') && !in_squote && !in_dquote)
			break ;
		token[i++] = *str++;
	}
	token[i] = '\0';
	while (*str == ' ' || *str == '\t')
		str++;
	*input_ptr = str;
	return (token);
}


char	**split_arguments(char *input)
{
	int		i;
	char	**args;
	char	*token;
	char	*input_ptr;
	int		total_tokens;
	char	*temp_input;
	char	*temp_ptr;

	if (!input)
		return (NULL);
	total_tokens = 0;
	input_ptr = input;
	temp_input = ft_strdup(input);
	temp_ptr = temp_input;
	while ((token = get_next_token(&temp_ptr)))
	{
		total_tokens++;
		free(token);
	}
	free(temp_input);
	args = malloc(sizeof(char *) * (total_tokens + 1));
	if (!args)
		return (NULL);
	input_ptr = input;
	i = 0;
	while ((token = get_next_token(&input_ptr)) && i < total_tokens)
	{
		args[i++] = ft_strdup(token);
		free(token);
	}
	args[i] = NULL;
	return (args);
}
