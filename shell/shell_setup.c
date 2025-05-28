/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_setup.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 11:43:51 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/28 01:20:48 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	build_prompt(char *prompt, t_shell *shell, const char *display_path)
{
	size_t	i;

	i = 0;
	i += ft_strlcpy(prompt + i, BOLD GREEN "[minishell]" RESET " ", PROMPT_MAX
			- i);
	i += ft_strlcpy(prompt + i, BOLD BLUE, PROMPT_MAX - i);
	i += ft_strlcpy(prompt + i, shell->user, PROMPT_MAX - i);
	i += ft_strlcpy(prompt + i, "@", PROMPT_MAX - i);
	i += ft_strlcpy(prompt + i, shell->hostname, PROMPT_MAX - i);
	i += ft_strlcpy(prompt + i, RESET ":", PROMPT_MAX - i);
	i += ft_strlcpy(prompt + i, display_path, PROMPT_MAX - i);
	ft_strlcpy(prompt + i, GREEN "$" RESET " ", PROMPT_MAX - i);
}

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

void	setup_shell(t_shell *shell, char **envp)
{
	int	env_count;

	signal(SIGPIPE, SIG_IGN);
	if (!shell || !envp)
		return ;
	env_count = 0;
	builtin_setup(shell->builtins);
	user_hostname(shell);
	while (envp[env_count])
		env_count++;
	allocate_env_memory(shell, env_count);
	if (!shell->envp || !shell->export_list)
		return ;
	duplicate_env_vars(shell, envp, env_count);
	add_or_update_env(shell, "OLDPWD", "");
	shell->is_prompting = FALSE;
	shell->should_exit = FALSE;
	//builtin_setup(shell->builtins); moved...
	shell->input.raw = NULL;
	shell->input.trim_spaces = NULL;
	shell->input.processed = NULL;
	shell->input.expanded = NULL;
	shell->input.args = NULL;
	shell->input.commands = NULL;
	//shell->input.heredoc_fd = -1;
}
