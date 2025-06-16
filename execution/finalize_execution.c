/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finalize_execution.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 01:58:56 by carlos-j          #+#    #+#             */
/*   Updated: 2025/06/16 09:38:42 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	setup_ignore_sigint(struct sigaction *old_sa)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, old_sa);
}

static void	handle_exit_status(t_shell *shell, int status)
{
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		shell->exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
			g_signal_status = WTERMSIG(status);
	}
}

int	finalize_execution(t_shell *shell, pid_t *child_pids, t_exec_state *state)
{
	int					i;
	int					status;
	struct sigaction	old_sa;

	setup_ignore_sigint(&old_sa);
	status = 0;
	safe_close(state->prev_read);
	i = 0;
	while (i < state->pid_idx)
	{
		waitpid(child_pids[i], &status, 0);
		if (i == state->pid_idx - 1)
			handle_exit_status(shell, status);
		i++;
	}
	sigaction(SIGINT, &old_sa, NULL);
	free(child_pids);
	return (1);
}
