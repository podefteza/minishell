/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:42:01 by carlos-j          #+#    #+#             */
/*   Updated: 2025/04/30 12:13:13 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_last_bg_pid(t_shell *shell)
{
	if (shell->last_bg_pid == -1)
	{
		ft_putchar_fd('\0', 1);
		return (NULL);
	}
	return (ft_itoa(shell->last_bg_pid));
}

char	*expand_env_variable(char **input, t_shell *shell)
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
	char	*var_value;
	//char	*quoted_value;

	(*input)++;
	if (**input == '\0' || **input == ' ' || **input == '\"' || **input == '\'')
		return (ft_strdup("$"));
	else if (**input == '?')
	{
		(*input)++;
		var_value = ft_itoa(shell->exit_status);
	}
	else if (**input == '!')
	{
		(*input)++;
		var_value = expand_last_bg_pid(shell);
	}
	else if (**input == '0')
	{
		(*input)++;
		var_value = ft_strdup(get_shell_name());
	}
	else
		var_value = expand_env_variable(input, shell);
	if (!var_value)
		return (ft_strdup("\"\""));
	// there was a specific case where we needed to add quotes...
	/*quoted_value = ft_strjoin("\"", var_value);
	free(var_value);
	var_value = ft_strjoin(quoted_value, "\"");
	free(quoted_value);*/
	return (var_value);
}
