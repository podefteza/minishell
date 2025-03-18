/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 11:51:22 by carlos-j          #+#    #+#             */
/*   Updated: 2025/03/18 14:05:10 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_shell_name(void)
{
	char	*shell_name;

	shell_name = getenv("_");
	if (shell_name)
		return (shell_name);
	else
		return ("shell name is not set");
}

void	expand_handle_quotes(char **input, char **ptr, int *in_single,
		int *in_double)
{
	if (**input == '\'' && !(*in_double))
		*in_single = !(*in_single);
	else if (**input == '\"' && !(*in_single))
		*in_double = !(*in_double);
	*(*ptr)++ = **input;
	(*input)++;
}

void	expand_process_input(char **input, char **ptr, t_shell *shell,
		int *in_single_quote)
{
	char	*var_value;
	int		in_double_quote;
	pid_t	shell_pid;

	shell_pid = getpid();
	in_double_quote = 0;
	while (**input)
	{
		if (**input == '\'' || **input == '\"')
			expand_handle_quotes(input, ptr, in_single_quote, &in_double_quote);
		else if (**input == '$' && !(*in_single_quote))
		{
			var_value = expand_dollar_sign(input, shell, shell_pid);
			if (var_value && *var_value != '\0')
			{
				ft_strlcpy(*ptr, var_value, ft_strlen(var_value) + 1);
				*ptr += ft_strlen(var_value);
			}
			free(var_value);
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
