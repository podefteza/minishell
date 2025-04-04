/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:23:59 by carlos-j          #+#    #+#             */
/*   Updated: 2025/04/04 16:30:33 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// move this... make use of it in other functions...............................................
int	is_redirection_token(char *token)
{
	return ((ft_strncmp(token, "<", 1) == 0 && ft_strlen(token) == 1)
		|| (ft_strncmp(token, "<<", 2) == 0 && ft_strlen(token) == 2)
		|| (ft_strncmp(token, ">", 1) == 0 && ft_strlen(token) == 1)
		|| (ft_strncmp(token, ">>", 2) == 0 && ft_strlen(token) == 2));
}

char	*get_next_token_for_echo(char **str)
{
	char	*start;
	char	*token;
	int		in_single_quote;
	int		in_double_quote;
	int		token_len;

	if (!str || !*str || **str == '\0')
		return (NULL);
	while (**str == ' ' || **str == '\t')
		(*str)++;
	if (**str == '\0')
		return (NULL);
	start = *str;
	in_single_quote = 0;
	in_double_quote = 0;
	token_len = 0;
	while (**str != '\0')
	{
		if (**str == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (**str == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		if (!in_single_quote && !in_double_quote && (**str == ' '
				|| **str == '\t'))
			break ;
		(*str)++;
		token_len++;
	}
	token = malloc(token_len + 1);
	if (!token)
		return (NULL);
	ft_strlcpy(token, start, token_len + 1);
	while (**str == ' ' || **str == '\t')
		(*str)++;
	return (token);
}

char	**handle_echo(char *modified_input, t_shell *shell)
{
	char	**args;
	char	*message;
	int		arg_count;
	char	*token;
	char	*tmp_input;
	char	*count_message;
	int		i;
	char	*cleaned_token;

	args = NULL;
	arg_count = 0;
	tmp_input = ft_strdup(modified_input + 4);
	if (!tmp_input)
		return (NULL);
	message = tmp_input;
	while (*message == ' ' || *message == '\t')
		message++;
	count_message = message;
	arg_count = 1;
	while ((token = get_next_token_for_echo(&count_message)))
	{
		arg_count++;
		free(token);
	}
	args = malloc(sizeof(char *) * (arg_count + 1));
	if (!args)
	{
		free(tmp_input);
		return (NULL);
	}
	args[0] = ft_strdup("echo");
	i = 1;
	message = tmp_input;
	while (*message == ' ' || *message == '\t')
		message++;
	while ((token = get_next_token_for_echo(&message)))
	{
		cleaned_token = ft_strdup(token);
		if (cleaned_token && cleaned_token[0] != '\'')
			cleaned_token = expand_variables(cleaned_token, shell);
		cleaned_token = handle_quotes(cleaned_token);
		args[i++] = cleaned_token;
		free(token);
	}
	args[i] = NULL;
	free(tmp_input);
	return (args);
}
