/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 13:21:18 by carlos-j          #+#    #+#             */
/*   Updated: 2025/06/18 11:24:56 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	execute_parent(pid_t *child_pids, t_exec_state *state, pid_t pid)
{
	signal(SIGPIPE, SIG_IGN);
	child_pids[state->pid_idx] = pid;
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

static int	is_invalid_command(t_shell *shell, t_exec_state *state)
{
	char	**args;

	if (!shell->input.commands || !shell->input.commands[state->cmd_idx])
		return (1);
	args = shell->input.commands[state->cmd_idx];
	if (!args[0] || (args[0][0] == '|' && args[0][1] == '\0'))
		return (1);
	return (0);
}

void	handle_command(t_shell *shell, pid_t *child_pids, t_exec_state *state)
{
	char	**args;
	pid_t	pid;

	if (is_invalid_command(shell, state))
	{
		state->cmd_idx += 1;
		return ;
	}
	args = shell->input.commands[state->cmd_idx];
	setup_next_pipe(shell, state->cmd_idx, state->pipe_fd);
	pid = fork();
	if (pid == 0)
	{
		free(child_pids);
		execute_child(shell, args, state->prev_read, state->pipe_fd);
	}
	else if (pid > 0)
		execute_parent(child_pids, state, pid);
	else
		perror("fork");
	state->cmd_idx++;
}
