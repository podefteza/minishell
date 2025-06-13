/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_setup.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 11:43:51 by carlos-j          #+#    #+#             */
/*   Updated: 2025/06/13 15:49:01 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	cleanup_and_exit(t_shell *shell)
{
	if (shell->envp)
		free(shell->envp);
	if (shell->export_list)
		free(shell->export_list);
	shell->envp = NULL;
	shell->export_list = NULL;
}

static void	duplicate_env_vars(t_shell *shell, char **envp, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		shell->envp[i] = ft_strdup(envp[i]);
		shell->export_list[i] = ft_strdup(envp[i]);
		if (!shell->envp[i] || !shell->export_list[i])
		{
			while (i-- > 0)
			{
				free(shell->envp[i]);
				free(shell->export_list[i]);
			}
			cleanup_and_exit(shell);
			return ;
		}
		i++;
	}
	shell->envp[count] = NULL;
	shell->export_list[count] = NULL;
}

static void	allocate_env_memory(t_shell *shell, int count)
{
	shell->envp = malloc((count + 1) * sizeof(char *));
	shell->export_list = malloc((count + 1) * sizeof(char *));
	if (!shell->envp || !shell->export_list)
	{
		cleanup_and_exit(shell);
		return ;
	}
}

static void	init_input_fields(t_shell *shell)
{
	shell->temp_files = NULL;
	shell->is_prompting = FALSE;
	shell->should_exit = FALSE;
	shell->input.raw = NULL;
	shell->input.trim_spaces = NULL;
	shell->input.processed = NULL;
	shell->input.expanded = NULL;
	shell->input.args = NULL;
	shell->input.commands = NULL;
}

void	setup_shell(t_shell *shell, char **envp)
{
	int		env_count;
	char	*init_oldpwd[3];

	if (!shell || !envp)
		return ;
	ft_memset(shell, 0, sizeof(t_shell));
	builtin_setup(shell->builtins);
	user_hostname(shell);
	env_count = 0;
	while (envp[env_count])
		env_count++;
	allocate_env_memory(shell, env_count);
	if (!shell->envp || !shell->export_list)
		return ;
	duplicate_env_vars(shell, envp, env_count);
	add_or_update_env(shell, "OLDPWD", "");
	init_oldpwd[0] = "export";
	init_oldpwd[1] = "OLDPWD=";
	init_oldpwd[2] = NULL;
	builtin_export(init_oldpwd, shell);
	update_shlvl(shell);
	init_input_fields(shell);
}
