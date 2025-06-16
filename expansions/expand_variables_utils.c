/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:49:52 by carlos-j          #+#    #+#             */
/*   Updated: 2025/06/16 21:34:14 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

static char	*input_with_expansion(t_shell *shell)
{
	char	*old;
	char	*trimmed;

	old = shell->input.processed;
	shell->input.processed = expand_variables(old, shell);
	free(old);
	if (!shell->input.processed)
		return (ft_strdup(""));
	trimmed = ft_strtrim(shell->input.processed, " ");
	free(shell->input.processed);
	return (trimmed);
}

static int	is_expansion_needed(char *str)
{
	int	i;
	int	in_single;
	int	in_double;

	i = -1;
	in_single = 0;
	in_double = 0;
	while (str[++i])
	{
		if (str[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (str[i] == '\"' && !in_single)
			in_double = !in_double;
		else if (str[i] == '$' && !in_single && (i == 0 || str[i - 1] != '\\'))
			return (1);
	}
	return (0);
}

void	check_for_expansion(t_shell *shell)
{
	char	*str;

	str = shell->input.processed;
	if (!is_expansion_needed(str))
	{
		shell->input.expanded = ft_strdup(str);
		free(shell->input.processed);
		shell->input.processed = NULL;
		return ;
	}
	shell->input.expanded = input_with_expansion(shell);
}
