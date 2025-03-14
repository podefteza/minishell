/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 11:51:22 by carlos-j          #+#    #+#             */
/*   Updated: 2025/03/14 10:38:48 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(char *var, t_shell *shell)
{
	int		i;
	char	*prefix;

	i = 0;
	prefix = ft_strjoin(var, "=");
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], prefix, ft_strlen(prefix)) == 0)
		{
			free(prefix);
			return (ft_strdup(shell->envp[i] + ft_strlen(var) + 1));
		}
		i++;
	}
	free(prefix);
	return (NULL);
}

char	*get_shell_name(void)
{
	char	*shell_name;

	shell_name = getenv("_");
	if (shell_name)
		return (shell_name);
	else
		return ("shell name is not set"); // or minishell?
}

char	*expand_variables(char *input, t_shell *shell)
{
	char	*result;
	char	*ptr;
	char	*var_value;
	int		in_single_quote;
	int		in_double_quote;
	char	var_name[100];
	int		i;
	pid_t	shell_pid;

	var_value = NULL;
	in_single_quote = 0;
	in_double_quote = 0;
	result = malloc(PATH_MAX);
	if (!result)
		return (NULL);
	ptr = result;
	while (*input)
	{
		if (*input == '\'' && !in_double_quote)
		{
			in_single_quote = !in_single_quote;
			*ptr++ = *input++;
		}
		else if (*input == '\"' && !in_single_quote)
		{
			in_double_quote = !in_double_quote;
			*ptr++ = *input++;
		}
		else if (*input == '$' && !in_single_quote)
		{
			input++;
			if (*input == '\0' || *input == ' ' || *input == '\"'
				|| *input == '\'')
				*ptr++ = '$';
			else if (*input == '?')
			{
				var_value = ft_itoa(shell->exit_status);
				input++;
			}
			else if (*input == '$')
			{
				shell_pid = getpid();
				var_value = ft_itoa(shell_pid);
				input++;
			}
			else if (*input == '!')
			{
				if (shell->last_bg_pid == -1)
				{
					ft_putchar_fd('\0', 1);
					var_value = NULL;
				}
				else
				{
					var_value = ft_itoa(shell->last_bg_pid);
				}
				input++;
			}
			else if (*input == '0')
			{
				var_value = ft_strdup(get_shell_name());
				input++;
			}
			else
			{
				i = 0;
				while (*input && (ft_isalnum(*input) || *input == '_'))
					var_name[i++] = *input++;
				var_name[i] = '\0';
				var_value = get_env_value(var_name, shell);
			}
			if (var_value && *var_value != '\0')
			{
				ft_strlcpy(ptr, var_value, ft_strlen(var_value) + 1);
				ptr += ft_strlen(var_value);
			}
			free(var_value);
			var_value = NULL;
		}
		else
			*ptr++ = *input++;
	}
	*ptr = '\0';
	return (result);
}
