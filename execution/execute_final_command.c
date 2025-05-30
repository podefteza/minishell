/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_final_command.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 02:15:42 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/30 15:26:37 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_pipeline(t_shell *shell)
{
	int	i;

	if (!shell->input.commands)
		return (FALSE);
	i = 0;
	while (shell->input.commands[i])
	{
		if (shell->input.commands[i][0]
			&& ft_strncmp(shell->input.commands[i][0], "|", 2) == 0)
		{
			return (TRUE);
		}
		i++;
	}
	return (FALSE);
}

static void	handle_exit_if_needed(t_shell *shell, int need_restore,
		int stdin_backup, int stdout_backup)
{
	if (!shell->input.commands || !shell->input.commands[0])
	{
		if (need_restore)
		{
			close(stdin_backup);
			close(stdout_backup);
		}
		return ;
	}

	if (ft_strncmp(shell->input.commands[0][0], "exit", 5) == 0
		&& !is_pipeline(shell))
	{
		if (need_restore)
		{
			close(stdin_backup);
			close(stdout_backup);
		}
		builtin_exit(shell->input.commands[0], shell);
	}
}


static int	handle_builtin_if_needed(t_shell *shell, int need_restore,
		int stdin_backup, int stdout_backup)
{
	if (!is_pipeline(shell) && execute_builtins(shell,
			shell->input.commands[0]))
	{
		if (shell->should_exit)
		{
			if (need_restore)
			{
				close(stdin_backup);
				close(stdout_backup);
			}
			builtin_exit(shell->input.commands[0], shell);
		}
		else if (need_restore)
			restore_stdio(stdout_backup, stdin_backup);
		return (1);
	}
	return (0);
}

void	execute_final_command(t_shell *shell)
{
	int	stdin_backup;
	int	stdout_backup;
	int	need_restore;

	if (!shell->input.commands || !shell->input.commands[0])
		return ;
	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	need_restore = TRUE;
	handle_exit_if_needed(shell, need_restore, stdin_backup, stdout_backup);
	if (shell->should_exit)
		return ;
	if (handle_builtin_if_needed(shell, need_restore, stdin_backup,
			stdout_backup))
		return ;
	execute_command(shell);
	if (need_restore)
		restore_stdio(stdout_backup, stdin_backup);
}
