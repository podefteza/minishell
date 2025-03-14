/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_processes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 14:45:59 by carlos-j          #+#    #+#             */
/*   Updated: 2025/03/14 15:10:09 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	setup_pipes(int *pipe_fds, char **commands, int i)
{
	if (commands[i + 1])
		pipe(pipe_fds);
}

static void	child_process(int in_fd, int *pipe_fds, char *command, t_shell *shell)
{
	char	**cmd;

	cmd = ft_split(command, ' ');
	if (!cmd || !cmd[0])
		exit(EXIT_FAILURE);
	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (pipe_fds[1] != -1)
	{
		dup2(pipe_fds[1], STDOUT_FILENO);
		close(pipe_fds[1]);
		close(pipe_fds[0]);
	}
	execute_command(cmd, shell);
	exit(EXIT_FAILURE);
}

static void	parent_process(int *in_fd, int *pipe_fds, int pid, int *status)
{
	waitpid(pid, status, 0);
	if (*in_fd != STDIN_FILENO)
		close(*in_fd);
	*in_fd = pipe_fds[0];
	close(pipe_fds[1]);
}

int	fork_processes(int *pipe_fds, char **commands, t_shell *shell)
{
	pid_t	pid;
	int		status;
	int		i;
	int		in_fd;

	status = 0;
	in_fd = STDIN_FILENO;
	if (!commands)
		return (1);
	i = 0;
	while (commands[i])
	{
		setup_pipes(pipe_fds, commands, i);
		pid = fork();
		if (pid == 0)
			child_process(in_fd, pipe_fds, commands[i], shell);
		else if (pid > 0)
			parent_process(&in_fd, pipe_fds, pid, &status);
		i++;
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}
