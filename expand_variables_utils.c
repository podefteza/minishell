/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:49:52 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/05 11:49:57 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*get_shell_name(void)
{
	char	*shell_name;

	shell_name = getenv("_");
	if (shell_name)
		return (ft_strdup(shell_name));
	else
		return (ft_strdup("shell name is not set"));
}

char	*input_with_expansion(char *final_input, t_shell *shell)
{
	char	*trimmed;

	final_input = expand_variables(final_input, shell);
	if (!final_input)
		return (ft_strdup(""));
	trimmed = ft_strtrim(final_input, " ");
	free(final_input);
	return (trimmed);
}

char	*check_for_expansion(char *final_input, t_shell *shell)
{
	char	*expanded;

	if (ft_strchr(final_input, '$'))
	{
		expanded = input_with_expansion(final_input, shell);
		free(final_input);
		return (expanded);
	}
	return (final_input);
}
