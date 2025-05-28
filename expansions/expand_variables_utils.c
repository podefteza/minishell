/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:49:52 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/28 13:44:40 by carlos-j         ###   ########.fr       */
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


void	check_for_expansion(t_shell *shell)
{
	int		i;
	int		in_single;
	int		needs_expansion;

	i = -1;
	in_single = 0;
	needs_expansion = 0;
	while (shell->input.processed[++i] && !needs_expansion)
	{
		if (shell->input.processed[i] == '\'')
			in_single = !in_single;
		else if (shell->input.processed[i] == '$' && !in_single
			&& (i == 0 || shell->input.processed[i - 1] != '\\'))
			needs_expansion = 1;
	}
	if (!needs_expansion)
	{
		shell->input.expanded = ft_strdup(shell->input.processed);
		free(shell->input.processed);
		shell->input.processed = NULL;
		return ;
	}
	shell->input.expanded = input_with_expansion(shell);
}
