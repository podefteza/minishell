/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 14:15:03 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/26 11:32:45 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	validate_preconditions(t_shell *shell, int stdin_backup,
		int stdout_backup)
{
	if (!shell->input.commands || stdin_backup == -1 || stdout_backup == -1)
	{
		if (stdin_backup != -1)
			close(stdin_backup);
		if (stdout_backup != -1)
			close(stdout_backup);
		return (0);
	}
	return (1);
}

static void	free_and_restore(int stdin_backup, int stdout_backup)
{
	if (stdin_backup != -1)
	{
		dup2(stdin_backup, STDIN_FILENO);
		close(stdin_backup);
	}
	if (stdout_backup != -1)
	{
		dup2(stdout_backup, STDOUT_FILENO);
		close(stdout_backup);
	}
}

static int	process_commands(t_shell *shell, pid_t *child_pids, int in_bak,
		int out_bak)
{
	t_exec_state	state;

	state.cmd_idx = 0;
	state.pid_idx = 0;
	state.prev_read = -1;
	while (shell->input.commands[state.cmd_idx])
		handle_command(shell, child_pids, &state);
	finalize_execution(shell, child_pids, &state);
	restore_stdio(out_bak, in_bak);
	return (1);
}

int	execute_command(t_shell *shell)
{
	int		stdin_backup;
	int		stdout_backup;
	int		num_commands;
	pid_t	*child_pids;

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	if (!validate_preconditions(shell, stdin_backup, stdout_backup))
		return (0);
	num_commands = count_executables(shell->input.commands);
	child_pids = malloc(num_commands * sizeof(pid_t));
	if (!child_pids)
		return (free_and_restore(stdin_backup, stdout_backup), 0);
	return (process_commands(shell, child_pids, stdin_backup, stdout_backup));
}
