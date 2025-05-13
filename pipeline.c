/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:32:02 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/12 21:58:09 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	wait_for_commands_and_set_status(pid_t *pids, int pid_count,
		t_shell *shell)
{
	int	i;
	int	status;

	i = 0;
	while (i < pid_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == pid_count - 1)
		{
			if (WIFEXITED(status))
				shell->exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				shell->exit_status = 128 + WTERMSIG(status);
		}
		i++;
	}
}

void	execute_pipeline(char **commands, t_shell *shell)
{
	int		input_fd;
	pid_t	pids[MAX_PIPELINE_CMDS];
	int		pid_count;
	int		cmd_count;

	input_fd = STDIN_FILENO;
	cmd_count = count_commands(commands);
	if (cmd_count > MAX_PIPELINE_CMDS)
	{
		ft_putstr_fd("minishell: pipeline too long\n", STDERR_FILENO);
		return ;
	}
	clean_command_args(commands);
	check_for_redirections(commands);
	pid_count = process_commands_in_pipeline(commands, &input_fd, pids, shell);
	if (pid_count == -1)
	{
		if (input_fd != STDIN_FILENO)
			close(input_fd);
		return ;
	}
	wait_for_commands_and_set_status(pids, pid_count, shell);
	if (input_fd != STDIN_FILENO)
		close(input_fd);
}
