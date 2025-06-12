/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:42:01 by carlos-j          #+#    #+#             */
/*   Updated: 2025/06/12 13:53:38 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*expand_env_variable(char **input, t_shell *shell)
{
	char	var_name[100];
	char	*prefix;
	int		i;
	int		j;

	i = 0;
	while (**input && (ft_isalnum(**input) || **input == '_'))
		var_name[i++] = *(*input)++;
	var_name[i] = '\0';
	prefix = ft_strjoin(var_name, "=");
	if (!prefix)
		return (NULL);
	j = 0;
	while (shell->envp[j])
	{
		if (ft_strncmp(shell->envp[j], prefix, ft_strlen(prefix)) == 0)
		{
			free(prefix);
			return (ft_strdup(shell->envp[j] + ft_strlen(var_name) + 1));
		}
		j++;
	}
	free(prefix);
	return (ft_strdup(""));
}

char	*expand_dollar_sign(char **input, t_shell *shell)
{
	(*input)++;
	if (**input == '\0' || **input == ' ' || **input == '\"' || **input == '\'')
		return (ft_strdup("$"));
	if (**input == '?')
	{
		(*input)++;
		return (ft_itoa(shell->exit_status));
	}
	if (**input == '0')
	{
		(*input)++;
		return (ft_strdup(get_shell_name()));
	}
	if (ft_isdigit(**input))
	{
		(*input)++;
		return (ft_strdup(""));
	}
	if (ft_isalnum(**input) || **input == '_')
		return (expand_env_variable(input, shell));
	return (ft_strdup("$"));
}
