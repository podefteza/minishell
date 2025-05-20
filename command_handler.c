/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 14:15:03 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/20 14:16:51 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*validate_and_find_command(char **args, t_shell *shell)
{
	char		*full_path;
	struct stat	st;

	if (!args[0] || args[0][0] == '\0')
		return (NULL);
	full_path = find_command(args[0], shell);
	if (!full_path)
		return (NULL);
	if (stat(full_path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		ft_puterr("minishell: ", full_path, IAD, "\n");
		shell->exit_status = 126;
		free(full_path);
		return (NULL);
	}
	return (full_path);
}

void	close_pipe_ends(int pipe_fd[2], int prev_read)
{
	if (pipe_fd[0] != -1)
		close(pipe_fd[0]);
	if (pipe_fd[1] != -1)
		close(pipe_fd[1]);
	if (prev_read != -1)
		close(prev_read);
}

bool	needs_pipe(t_shell *shell, int idx)
{
	return (shell->input.commands[idx + 1] && shell->input.commands[idx + 1][0]
		&& ft_strncmp(shell->input.commands[idx + 1][0], "|", 2) == 0);
}

void	handle_child_execution(char **args, t_shell *shell)
{
	char	*full_path;

	fprintf(stderr, "entering handle_child_execution....................\n");

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);

	full_path = validate_and_find_command(args, shell);
	fprintf(stderr, "Trying to execve: %s\n", full_path);
	if (!full_path)
	{
		free_shell_resources(shell);
		free(full_path);
		free_array(args);
		exit(shell->exit_status); // added this!!!!!!!!!!!!!!!!
	}


	execve(full_path, args, shell->envp);
	perror("execve");
	if (full_path != args[0])
		free(full_path);
	shell->exit_status = 126;
	free_shell_resources(shell);
	exit(shell->exit_status);
}


// Helper function to safely close file descriptors
void	safe_close(int fd)
{
	if (fd != -1)
		close(fd);
}

int	is_builtin(char *cmd, t_shell *shell)
{
	if (!cmd)
		return (0);
	for (int i = 0; shell->builtins[i].cmd; i++)
	{
		if (ft_strncmp(cmd, shell->builtins[i].cmd, ft_strlen(cmd)) == 0)
			return (1);
	}
	return (0);
}

int	execute_command(t_shell *shell)
{
	int		stdin_backup;
	int		stdout_backup;
	int		pipe_fd[2] = {-1, -1};
	int		prev_pipe_read;
	int		status;
	pid_t	*child_pids;
	int		num_commands;
	int		cmd_idx;
	int		pid_idx;
	int		child_status;
	char	**args;
	pid_t	pid;

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	prev_pipe_read = -1;
	status = 1;
	child_pids = NULL;
	num_commands = 0;
	cmd_idx = 0;
	pid_idx = 0;
	if (!shell->input.commands || stdin_backup == -1 || stdout_backup == -1)
	{
		if (stdin_backup != -1)
			close(stdin_backup);
		if (stdout_backup != -1)
			close(stdout_backup);
		return (0);
	}
	for (int i = 0; shell->input.commands[i]; i++)
	{
		if (!(shell->input.commands[i][0]
				&& ft_strncmp(shell->input.commands[i][0], "|", 2) == 0))
		{
			num_commands++;
		}
	}
	child_pids = malloc(num_commands * sizeof(pid_t));
	if (!child_pids)
	{
		close(stdin_backup);
		close(stdout_backup);
		return (0);
	}
	while (shell->input.commands[cmd_idx])
	{
		args = shell->input.commands[cmd_idx];
		if (args && args[0] && ft_strncmp(args[0], "|", 2) == 0)
		{
			cmd_idx++;
			continue ;
		}
		pipe_fd[0] = -1;
		pipe_fd[1] = -1;
		int lookahead = cmd_idx + 1;
		while (shell->input.commands[lookahead]
			&& shell->input.commands[lookahead][0]
			&& ft_strncmp(shell->input.commands[lookahead][0], "|", 2) == 0)
		{
			lookahead++;
		}
		if (shell->input.commands[lookahead] != NULL)
		{
			if (pipe(pipe_fd) == -1)
			{
				perror("pipe");
				status = -1;
				break;
			}
		}
		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			status = -1;
			break ;
		}
		else if (pid == 0)
		{
			// Child process
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			if (prev_pipe_read != -1)
			{
				dup2(prev_pipe_read, STDIN_FILENO);
				close(prev_pipe_read);
			}
			if (pipe_fd[1] != -1)
			{
				dup2(pipe_fd[1], STDOUT_FILENO);
				close(pipe_fd[1]);
			}

			// Handle redirections
			if (handle_redirections(args, shell) == -1)
				exit(1);

			if (is_builtin(args[0], shell))
			{
				execute_builtins(shell, args);
				free_shell_resources(shell);
				exit(shell->exit_status);  // This is correct
			}

			// Close remaining FDs before exec
			safe_close(pipe_fd[0]);
			safe_close(prev_pipe_read);
			handle_child_execution(args, shell);
		}
		else
		{
			child_pids[pid_idx++] = pid;
			safe_close(prev_pipe_read);
			safe_close(pipe_fd[1]);
			prev_pipe_read = pipe_fd[0];
		}
		cmd_idx++;
	}
	safe_close(prev_pipe_read);
	safe_close(pipe_fd[0]);
	safe_close(pipe_fd[1]);
	for (int i = 0; i < pid_idx; i++)
	{
		waitpid(child_pids[i], &child_status, 0);
		if (i == pid_idx - 1 && WIFEXITED(child_status))
			shell->exit_status = WEXITSTATUS(child_status);
	}

	free(child_pids);
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);
	printf("will return status %d\n", status);
	fprintf(stderr, "execute_command: final exit status = %d\n", shell->exit_status);
	return (status);
}
