/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:23:59 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/28 02:07:28 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	get_token_length(char **str, int *in_squote, int *in_dquote)
{
	int	len;

	len = 0;
	while (**str)
	{
		if (**str == '\'' && !*in_dquote)
			*in_squote = !*in_squote;
		else if (**str == '"' && !*in_squote)
			*in_dquote = !*in_dquote;
		if (!*in_squote && !*in_dquote && (**str == ' ' || **str == '\t'))
			break ;
		(*str)++;
		len++;
	}
	return (len);
}

static char	*get_next_token_for_echo(char **str)
{
	char	*start;
	char	*token;
	int		len;
	int		in_squote;
	int		in_dquote;

	if (!str || !*str || **str == '\0')
		return (NULL);
	while (**str == ' ' || **str == '\t')
		(*str)++;
	if (**str == '\0')
		return (NULL);
	start = *str;
	in_squote = 0;
	in_dquote = 0;
	len = get_token_length(str, &in_squote, &in_dquote);
	token = malloc(len + 1);
	if (!token)
		return (NULL);
	ft_strlcpy(token, start, len + 1);
	while (**str == ' ' || **str == '\t')
		(*str)++;
	return (token);
}

int	count_args_for_echo(char *message)
{
	int		arg_count;
	char	*token;
	char	*count_message;

	arg_count = 1;
	count_message = message;
	token = get_next_token_for_echo(&count_message);
	while (token != NULL)
	{
		arg_count++;
		free(token);
		token = get_next_token_for_echo(&count_message);
	}
	return (arg_count);
}
