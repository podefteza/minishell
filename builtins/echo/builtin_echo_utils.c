/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:23:59 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/12 19:35:46 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	get_token_length(char **str, int *in_squote, int *in_dquote)
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

char	*get_next_token_for_echo(char **str)
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

char	*handle_dollar_quotes(char *token, t_shell *shell)
{
	char	*result;

	if (!token)
		return (NULL);
	if (token[0] == '$' && token[1] == '"')
	{
		result = malloc(ft_strlen(token));
		if (!result)
			return (NULL);
		ft_strlcpy(result, token + 1, ft_strlen(token));
		free(token);
		token = expand_variables(result, shell);
		return (token);
	}
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

void	process_echo_tokens(char **args, char *message, t_shell *shell)
{
	char	*token;
	char	*cleaned_token;
	int		i;

	i = 1;
	token = get_next_token_for_echo(&message);
	while (token != NULL)
	{
		cleaned_token = ft_strdup(token);
		if (cleaned_token && cleaned_token[0] == '$' && cleaned_token[1] == '"')
			cleaned_token = handle_dollar_quotes(cleaned_token, shell);
		else if (cleaned_token && cleaned_token[0] != '\'')
			cleaned_token = expand_variables(cleaned_token, shell);
		args[i++] = handle_quotes(cleaned_token);
		free(token);
		token = get_next_token_for_echo(&message);
	}
	args[i] = NULL;
}
