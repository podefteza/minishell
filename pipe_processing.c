/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_processing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 14:22:40 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/14 10:27:51 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**get_command_args(char *command)
{
	char	**args;

	if (is_empty(command))
	{
		args = malloc(sizeof(char *) * 2);
		if (!args)
			return (NULL);
		args[0] = ft_strdup(command);
		args[1] = NULL;
	}
	else
		args = split_arguments(command);
	return (args);
}

static int	setup_process_io(char *next_cmd, int *pipe_fds)
{
	int	orig_stdin;
	int	orig_stdout;

	if (setup_io_backups(&orig_stdin, &orig_stdout) == -1)
		return (-1);
	if (create_pipe_if_needed(next_cmd, pipe_fds) == -1)
		return (-1);
	restore_io(orig_stdin, orig_stdout);
	return (0);
}

// we need to change this to 4 args only....................... maybe change the shell struct to have the **commands array
static pid_t	fork_and_execute(char **args, int *input_fd, int pipe_fds[2],
		t_shell *shell, char **commands)
{
	pid_t	pid;

	/*printf("==================fork_and_execute: %s==================\n", args[0]);
	// print args
	for (int i = 0; args[i]; i++)
	{
		printf("args[%d]: %s\n", i, args[i]);
	}*/

	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		return (-1);
	}
	//printf("pid of the forked process: %d\n", pid);
	if (pid == 0)
	{
		// maybe we can free stuff here...? like input and commands
		free_array(commands); // added this...
		execute_child_process(args, *input_fd, pipe_fds, shell);
		//free_array(commands); // added this...
	}
	else
	{
		//printf("will execute_parent_process\n");
		execute_parent_process(input_fd, pipe_fds);
		//free_array(commands);
	}
	return (pid);
}

int	process_command(char **commands, int i, int *input_fd, t_shell *shell)
{
	int		pipe_fds[2];
	pid_t	pid;
	char	**args;

	pipe_fds[0] = -1;
	pipe_fds[1] = -1;

	// print commands
	/*for (int j = 0; commands[j]; j++)
	{
		printf("commands before [%d]: %s\n", j, commands[j]);
	}*/

	args = get_command_args(commands[i]);
	if (!args)
		return (error_return("minishell: memory allocation error\n", -1));
	// print args
	/*for (int j = 0; args[j]; j++)
	{
		printf("args after.........[%d]: %s\n", j, args[j]);
	}*/
	if (setup_process_io(commands[i + 1], pipe_fds) == -1)
	{
		//printf("its -1................\n");
		close_fds(pipe_fds, args);
		return (-1);
	}
	//free_array(commands); // added this...
	pid = fork_and_execute(args, input_fd, pipe_fds, shell, commands);
	//free_array(commands); // added this...
	if (pid == -1)
		close_fds(pipe_fds, args);

	//printf("will free array of args\n");
	free_array(args);
	return (pid);
}

int	process_commands_in_pipeline(char **commands, int *input_fd, pid_t *pids,
		t_shell *shell)
{
	int		i;
	int		pid_count;
	pid_t	pid;

	i = 0;
	pid_count = 0;
	while (commands[i] != NULL)
	{
		pid = process_command(commands, i, input_fd, shell);
		//printf("pid of the process: %d\n", pid);
		if (pid == -1)
		{
			if (*input_fd != STDIN_FILENO)
				close(*input_fd);
			return (-1);
		}
		//waitpid(pid, NULL, 0);
		pids[pid_count++] = pid;
		i++;
		//if (pid > 0)
		//	free_array(commands); // added this...

	}
	return (pid_count);
}
