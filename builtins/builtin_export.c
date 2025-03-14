/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 09:14:15 by carlos-j          #+#    #+#             */
/*   Updated: 2025/03/14 11:43:24 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (FALSE);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (FALSE);
		i++;
	}
	return (TRUE);
}

int	find_env_var(t_shell *shell, const char *key)
{
	int		i;
	size_t	key_len;

	if (!shell->envp || !key)
		return (-1);
	key_len = ft_strlen(key);
	i = 0;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], key, key_len) == 0
			&& shell->envp[i][key_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

char	**add_or_update_env(t_shell *shell, const char *key, const char *value)
{
	int		existing_index;
	char	*new_entry;
	char	**new_envp;
	char	*tmp;

	int i, env_count;
	existing_index = find_env_var(shell, key);
	env_count = 0;
	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (NULL);
	new_entry = ft_strjoin(tmp, value);
	free(tmp);
	if (!new_entry)
		return (NULL);
	if (existing_index != -1)
	{
		free(shell->envp[existing_index]);
		shell->envp[existing_index] = new_entry;
		return (shell->envp);
	}
	while (shell->envp[env_count])
		env_count++;
	new_envp = malloc((env_count + 2) * sizeof(char *));
	if (!new_envp)
	{
		free(new_entry);
		return (shell->envp);
	}
	i = 0;
	while (i < env_count)
	{
		new_envp[i] = shell->envp[i];
		i++;
	}
	new_envp[env_count] = new_entry;
	new_envp[env_count + 1] = NULL;
	free(shell->envp);
	return (new_envp);
}

void	sort_export(char **envp)
{
	int		swapped;
	int		i;
	char	*temp;

	swapped = TRUE;
	while (swapped)
	{
		swapped = FALSE;
		i = 0;
		while (envp[i + 1])
		{
			if (strcmp(envp[i], envp[i + 1]) > 0)
			{
				temp = envp[i];
				envp[i] = envp[i + 1];
				envp[i + 1] = temp;
				swapped = TRUE;
			}
			i++;
		}
	}
}

void	builtin_export(char **args, t_shell *shell)
{
	int		i;
	char	*key;
	char	*value;
	char	*equal_sign;
	int		env_count;

	if (!args[1])
	{
		env_count = 0;
		while (shell->envp[env_count])
			env_count++;
		sort_export(shell->envp);
		i = 0;
		while (shell->envp[i])
		{
			printf("declare -x %s\n", shell->envp[i]);
			i++;
		}
		return ;
	}
	i = 1;
	while (args[i])
	{
		equal_sign = ft_strchr(args[i], '=');
		if (equal_sign)
		{
			*equal_sign = '\0';
			key = args[i];
			value = equal_sign + 1;
			if (!is_valid_identifier(key))
			{
				ft_putstr_fd("minishell: export: `", 2);
				ft_putstr_fd(args[i], 2);
				ft_putstr_fd("': not a valid identifier\n", 2);
				shell->exit_status = 1;
			}
			else
				shell->envp = add_or_update_env(shell, key, value);
			*equal_sign = '=';
		}
		else if (is_valid_identifier(args[i]))
			shell->envp = add_or_update_env(shell, args[i], "");
		else
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			shell->exit_status = 1;
		}
		i++;
	}
}
