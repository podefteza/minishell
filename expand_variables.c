/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 11:51:22 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/09 11:32:30 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	append_var_value(char **ptr, char *var_value)
{
	if (var_value)
	{
		ft_strlcpy(*ptr, var_value, ft_strlen(var_value) + 1);
		*ptr += ft_strlen(var_value);
		free(var_value);
	}
}

void	handle_dollar_inside_quotes(char **input, char **ptr, t_shell *shell)
{
	char	*var_value;

	var_value = expand_dollar_sign(input, shell);
	if (var_value)
		append_var_value(ptr, var_value);
	else if (**input)
		*(*ptr)++ = *(*input)++;
}

void	expand_dollar_quoted_block(char **input, char **ptr, t_shell *shell,
		int *in_single_quote)
{
	char	quote;

	(*input)++;
	quote = *(*input)++;
	*(*ptr)++ = quote;
	while (**input && **input != quote)
	{
		if (**input == '$' && !(*in_single_quote))
			handle_dollar_inside_quotes(input, ptr, shell);
		else
			*(*ptr)++ = *(*input)++;
	}
	if (**input == quote)
	{
		*(*ptr)++ = quote;
		(*input)++;
	}
}

void	expand_process_input(char **input, char **ptr, t_shell *shell,
		int *in_single_quote)
{
	char	*var_value;
	int		in_double_quote;

	in_double_quote = 0;
	while (**input)
	{
		if (**input == '\'' || **input == '\"')
			expand_handle_quotes(input, ptr, in_single_quote, &in_double_quote);
		else if (**input == '$' && (*(*input + 1) == '\''
				|| *(*input + 1) == '\"') && (*(*input + 2)))
			expand_dollar_quoted_block(input, ptr, shell, in_single_quote);
		else if (**input == '$' && !(*in_single_quote))
		{
			var_value = expand_dollar_sign(input, shell);
			append_var_value(ptr, var_value);
		}
		else
			*(*ptr)++ = *(*input)++;
	}
}

char	*expand_variables(char *input, t_shell *shell)
{
	char	*result;
	char	*ptr;
	int		in_single_quote;

	in_single_quote = 0;
	result = malloc(PATH_MAX);
	if (!result)
		return (NULL);
	ptr = result;
	expand_process_input(&input, &ptr, shell, &in_single_quote);
	*ptr = '\0';
	return (result);
}
