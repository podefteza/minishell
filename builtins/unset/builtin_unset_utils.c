/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 11:20:59 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/29 01:49:29 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	**alloc_new_env(int env_count, t_shell *shell)
{
	char	**new_envp;

	if (env_count <= 0)
	{
		shell->exit_status = 1;
		return (NULL);
	}
	new_envp = malloc(env_count * sizeof(char *));
	if (!new_envp)
	{
		shell->exit_status = 1;
		return (NULL);
	}
	return (new_envp);
}

static char	**create_new_env(t_shell *shell, int env_index, int env_count)
{
	char	**new_envp;
	int		j;
	int		k;

	new_envp = alloc_new_env(env_count, shell);
	if (!new_envp)
		return (NULL);
	j = 0;
	k = 0;
	while (shell->envp[k])
	{
		if (k != env_index)
		{
			new_envp[j] = shell->envp[k];
			j++;
		}
		else
			free(shell->envp[k]);
		k++;
	}
	new_envp[j] = NULL;
	return (new_envp);
}

void	remove_env_var(t_shell *shell, char *var_name)
{
	int		env_index;
	char	**new_envp;
	int		env_count;

	env_index = find_env_var(shell, var_name);
	if (env_index != -1)
	{
		env_count = 0;
		while (shell->envp[env_count])
			env_count++;
		if (env_count <= 0)
			return ;
		new_envp = create_new_env(shell, env_index, env_count);
		if (!new_envp)
			return ;
		free(shell->envp);
		shell->envp = new_envp;
	}
}

static char	**create_new_export(t_shell *shell, int export_index,
		int export_count)
{
	char	**new_export;
	int		j;
	int		k;

	new_export = malloc(export_count * sizeof(char *));
	if (!new_export)
	{
		shell->exit_status = 1;
		return (NULL);
	}
	j = 0;
	k = -1;
	while (shell->export_list[++k])
	{
		if (k != export_index)
			new_export[j++] = shell->export_list[k];
		else
			free(shell->export_list[k]);
	}
	new_export[j] = NULL;
	return (new_export);
}

void	remove_export_var(t_shell *shell, char *var_name)
{
	int		export_index;
	char	**new_export_list;
	int		export_count;

	export_index = find_export_var(shell->export_list, var_name);
	if (export_index != -1)
	{
		export_count = 0;
		while (shell->export_list[export_count])
			export_count++;
		if (export_count <= 0)
			return ;
		new_export_list = create_new_export(shell, export_index, export_count);
		if (!new_export_list)
			return ;
		free(shell->export_list);
		shell->export_list = new_export_list;
	}
}
