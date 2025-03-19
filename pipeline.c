/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:32:02 by carlos-j          #+#    #+#             */
/*   Updated: 2025/03/19 19:29:14 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_processes(int *input_fd, int pipe_fds[2], char *next_command,
		int is_child)
{
	if (is_child)
	{
		if (*input_fd != STDIN_FILENO)
		{
			dup2(*input_fd, STDIN_FILENO);
			close(*input_fd);
		}
		if (next_command != NULL)
		{
			dup2(pipe_fds[1], STDOUT_FILENO);
			close(pipe_fds[1]);
		}
	}
	else
	{
		if (next_command != NULL)
			close(pipe_fds[1]);
		if (*input_fd != STDIN_FILENO)
			close(*input_fd);
		*input_fd = pipe_fds[0];
	}
}

void	execute_pipeline_command(char **args, t_shell *shell)
{
	if (args && args[0])
	{
		if (ft_strncmp(args[0], "env", 4) == 0)
		{
			builtin_env(args, shell);
			exit(EXIT_SUCCESS);
		}
		execvp(args[0], args);
		ft_putstr_fd(args[0], STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror("");
	}
}

int	initial_checks(char **args, t_shell *shell, char *next_command,
		int *pipe_fds)
{
	if (!check_command_access(args, shell) || (next_command
			&& setup_pipe(pipe_fds) == -1))
	{
		free_array(args);
		return (-1);
	}
	return (0);
}

int	process_command(char **commands, int i, int *input_fd, t_shell *shell)
{
	int		pipe_fds[2];
	pid_t	pid;
	char	**args;
	char	*next_command;

	args = split_arguments(commands[i]);
	next_command = commands[i + 1];
	if (initial_checks(args, shell, next_command, pipe_fds) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		free_array(args);
		return (-1);
	}
	if (pid == 0)
	{
		handle_processes(input_fd, pipe_fds, next_command, 1);
		execute_pipeline_command(args, shell);
		exit(EXIT_FAILURE);
	}
	handle_processes(input_fd, pipe_fds, next_command, 0);
	free_array(args);
	return (0);
}

void	execute_pipeline(char **commands, t_shell *shell)
{
	int	input_fd;
	int	i;

	input_fd = STDIN_FILENO;
	i = 0;
	while (commands[i] != NULL)
	{
		if (process_command(commands, i, &input_fd, shell) == -1)
			return ;
		i++;
	}
	while (wait(NULL) > 0)
		;
	shell->exit_status = 0;
}
