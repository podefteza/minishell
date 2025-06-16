/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 11:51:22 by carlos-j          #+#    #+#             */
/*   Updated: 2025/06/16 21:34:19 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	append_var_value(char **ptr, char *var_value)
{
	int	len;

	if (var_value)
	{
		len = ft_strlen(var_value);
		if (len > 0)
		{
			ft_strlcpy(*ptr, var_value, len + 1);
			*ptr += len;
		}
		free(var_value);
	}
}

static void	expand_process_input(char **input, char **ptr, t_shell *shell,
		int *in_single_quote)
{
	char	*var_value;
	int		in_double_quote;
	char	next_char;

	in_double_quote = 0;
	while (**input)
	{
		if (**input == '\'' || **input == '\"')
			expand_handle_quotes(input, ptr, in_single_quote, &in_double_quote);
		else if (**input == '$' && !(*in_single_quote))
		{
			next_char = 0;
			var_value = expand_dollar_sign(input, shell);
			append_var_value(ptr, var_value);
			if (next_char && !(ft_isalnum(next_char) || next_char == '_'))
				*(*ptr)++ = next_char;
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
