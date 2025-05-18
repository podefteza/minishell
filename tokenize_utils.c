/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 17:11:38 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/16 09:40:38 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*create_long_redir_token(char **str)
{
	char	first;
	int		count;
	char	*token;
	int		i;

	first = **str;
	count = 0;
	while ((*str)[count] == first)
		count++;
	token = malloc(count + 1);
	if (!token)
		return (NULL);
	i = 0;
	while (i < count)
	{
		token[i] = **str;
		(*str)++;
		i++;
	}
	token[i] = '\0';
	return (token);
}

static char	*create_short_redir_token(char **str)
{
	char	*token;
	int		i;

	token = malloc(3);
	if (!token)
		return (NULL);
	i = 0;
	token[i++] = *(*str)++;
	if ((**str == '>' && token[0] == '>') || (**str == '<' && token[0] == '<'))
		token[i++] = *(*str)++;
	token[i] = '\0';
	return (token);
}

static char	*read_normal_token(char **str)
{
	char	*token;
	int		in_squote;
	int		in_dquote;
	int		i;

	token = malloc(ft_strlen(*str) + 1);
	if (!token)
		return (NULL);
	in_squote = 0;
	in_dquote = 0;
	i = 0;
	while (**str)
	{
		if (**str == '\'' && !in_dquote)
			in_squote = !in_squote;
		else if (**str == '\"' && !in_squote)
			in_dquote = !in_dquote;
		else if ((**str == ' ' || **str == '<' || **str == '>') && !in_squote
			&& !in_dquote)
			break ;
		token[i++] = *(*str)++;
	}
	token[i] = '\0';
	return (token);
}

char	*get_next_token(char **input_ptr)
{
	char	*str;
	char	*token;

	if (!input_ptr || !*input_ptr)
		return (NULL);
	str = *input_ptr;
	str = skip_whitespace(str);
	if (*str == '\0')
		return (NULL);
	if (*str == '<' || *str == '>')
	{
		if (str[0] == str[1] && str[1] == str[2])
			token = create_long_redir_token(&str);
		else
			token = create_short_redir_token(&str);
	}
	else
		token = read_normal_token(&str);
	str = skip_whitespace(str);
	*input_ptr = str;
	return (token);
}
