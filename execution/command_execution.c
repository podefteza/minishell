/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 13:21:18 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/21 14:49:38 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	execute_parent(pid_t *pids, t_exec_state *state, pid_t pid)
{
	pids[state->pid_idx] = pid;
	state->pid_idx++;
	safe_close(state->prev_read);
	safe_close(state->pipe_fd[1]);
	state->prev_read = state->pipe_fd[0];
}

static void	setup_next_pipe(t_shell *shell, int i, int pipe_fd[2])
{
	int	lookahead;

	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	lookahead = i + 1;
	while (shell->input.commands[lookahead]
		&& shell->input.commands[lookahead][0]
		&& ft_strncmp(shell->input.commands[lookahead][0], "|", 2) == 0)
		lookahead++;
	if (shell->input.commands[lookahead])
		if (pipe(pipe_fd) == -1)
			perror("pipe");
}

void	handle_command(t_shell *shell, pid_t *pids, t_exec_state *state)
{
	char	**args;
	pid_t	pid;

	args = shell->input.commands[state->cmd_idx];
	if (!args || (args[0] && ft_strncmp(args[0], "|", 2) == 0))
		return ((void)(state->cmd_idx += 1));
	setup_next_pipe(shell, state->cmd_idx, state->pipe_fd);
	pid = fork();
	if (pid == 0)
		execute_child(shell, args, state->prev_read, state->pipe_fd);
	else if (pid > 0)
		execute_parent(pids, state, pid);
	else
		perror("fork");
	state->cmd_idx++;
}

int	finalize_execution(t_shell *shell, pid_t *pids, t_exec_state *state)
{
	int	i;
	int	status;

	safe_close(state->prev_read);
	i = 0;
	while (i < state->pid_idx)
	{
		waitpid(pids[i], &status, 0);
		if (i == state->pid_idx - 1 && WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
		i++;
	}
	free(pids);
	return (1);
}
