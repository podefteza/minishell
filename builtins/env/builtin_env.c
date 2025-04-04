/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 14:10:50 by carlos-j          #+#    #+#             */
/*   Updated: 2025/04/04 16:13:01 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*create_env_entry(const char *key, const char *value)
{
	char	*tmp;
	char	*new_entry;

	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (NULL);
	new_entry = ft_strjoin(tmp, value);
	free(tmp);
	return (new_entry);
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

char	**update_envp(t_shell *shell, char *new_entry, int existing_index)
{
	char	**new_envp;
	int		env_count;
	int		i;

	if (existing_index != -1)
	{
		free(shell->envp[existing_index]);
		shell->envp[existing_index] = new_entry;
		return (shell->envp);
	}
	env_count = 0;
	while (shell->envp[env_count])
		env_count++;
	new_envp = malloc((env_count + 2) * sizeof(char *));
	if (!new_envp)
		return (NULL);
	i = -1;
	while (++i < env_count)
		new_envp[i] = shell->envp[i];
	new_envp[env_count] = new_entry;
	new_envp[env_count + 1] = NULL;
	free(shell->envp);
	return (new_envp);
}

char	**add_or_update_env(t_shell *shell, const char *key, const char *value)
{
	int		existing_index;
	char	*new_entry;
	char	**new_envp;

	existing_index = find_env_var(shell, key);
	new_entry = create_env_entry(key, value);
	if (!new_entry)
		return (shell->envp);
	new_envp = update_envp(shell, new_entry, existing_index);
	if (!new_envp)
	{
		free(new_entry);
		return (shell->envp);
	}
	return (new_envp);
}

int	builtin_env(char **args, t_shell *shell)
{
	int	i;

	(void)args;
	if (!shell || !shell->envp)
	{
		ft_putstr_fd("env: environment not set\n", 2);
		shell->exit_status = 1;
		return (1);  // Return failure
	}
	i = 0;
	while (shell->envp[i])
	{
		printf("%s\n", shell->envp[i]);
		i++;
	}
	shell->exit_status = 0;
	return (0);  // Return success
}

