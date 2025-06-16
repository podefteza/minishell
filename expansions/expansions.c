/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:42:01 by carlos-j          #+#    #+#             */
/*   Updated: 2025/06/16 15:49:43 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*wrap_in_quotes(const char *str)
{
	char	*quoted;
	int		len;
	int		i;

	len = ft_strlen(str);
	quoted = malloc(len + 3);
	if (!quoted)
		return (NULL);
	quoted[0] = '"';
	i = 0;
	while (str[i])
	{
		quoted[i + 1] = str[i];
		i++;
	}
	quoted[len + 1] = '"';
	quoted[len + 2] = '\0';
	return (quoted);
}

char	*find_env_value(char *var_name, t_shell *shell)
{
	char	*prefix;
	char	*value;
	int		j;

	prefix = ft_strjoin(var_name, "=");
	if (!prefix)
		return (NULL);
	j = 0;
	while (shell->envp[j])
	{
		if (ft_strncmp(shell->envp[j], prefix, ft_strlen(prefix)) == 0)
		{
			value = shell->envp[j] + ft_strlen(var_name) + 1;
			free(prefix);
			return (value);
		}
		j++;
	}
	free(prefix);
	return (NULL);
}

static char	*expand_env_variable(char **input, t_shell *shell)
{
	char	var_name[100];
	char	*value;
	char	*result;
	int		i;

	i = 0;
	while (**input && (ft_isalnum(**input) || **input == '_'))
		var_name[i++] = *(*input)++;
	var_name[i] = '\0';
	value = find_env_value(var_name, shell);
	if (!value)
		return (ft_strdup(""));
	if (is_redirection_operator(value) || ft_strchr(value, '|'))
	{
		result = wrap_in_quotes(value);
		if (!result)
			result = ft_strdup("");
		return (result);
	}
	else
		return (ft_strdup(value));
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
