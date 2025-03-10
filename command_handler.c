/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 14:15:03 by carlos-j          #+#    #+#             */
/*   Updated: 2025/03/10 17:02:50 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	command_not_found(char **args, int *exit_status)
{
	if (*exit_status != 42126 && *exit_status != 42127)
	{
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": command not found", 2);
		ft_putstr_fd("\n", 2);
		*exit_status = 127;
	}
	if (*exit_status == 42126)
		*exit_status = 126;
	if (*exit_status == 42127)
		*exit_status = 127;
}

char	*find_command(char *cmd, char **envp, int *exit_status)
{
	char	*path;
	char	*full_path;

	full_path = cmd_is_path(cmd, exit_status);
	if (full_path)
		return (full_path);
	path = get_path_from_env(envp);
	if (!path)
	{
		command_not_found(&cmd, exit_status);
		return (NULL);
	}
	full_path = search_in_path(path, cmd);
	if (!full_path)
	{
		command_not_found(&cmd, exit_status);
		return (NULL);
	}
	return (full_path);
}

void	is_directory(char *full_path, int *exit_status)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(full_path, 2);
	ft_putstr_fd(": Is a directory\n", 2);
	*exit_status = 126;
}

void	execute_process(char *full_path, char **args, char **envp,
	int *exit_status, int is_background, pid_t *last_bg_pid)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return ;
	}
	if (pid == 0) // Child process
	{
		execve(full_path, args, envp);
		perror("execve"); // Only reached if execve fails
		exit(1);
	}
	else // Parent process
	{
		if (is_background)
		{
			*last_bg_pid = pid; // Store last background PID
			printf("[%d] %s started in background\n", pid, args[0]);
		}
		else
		{
			waitpid(pid, &status, 0);  // Wait for foreground process
			if (WIFEXITED(status))
				*exit_status = WEXITSTATUS(status);
		}
	}
}


void	execute_command(char **args, char **envp, int *exit_status, pid_t *last_bg_pid)
{
	char		*full_path;
	struct stat	st;
	int			arg_count;
	int			is_background;

	if (!args || !args[0])
		return ;

	// Count arguments to check for '&'
	arg_count = 0;
	while (args[arg_count])
		arg_count++;

	// Check if last argument is '&'
	is_background = 0;
	if (ft_strncmp(args[arg_count - 1], "&", 2) == 0)
	{
		is_background = 1;
		free(args[arg_count - 1]); // Remove '&' from arguments
		args[arg_count - 1] = NULL;
	}

	full_path = find_command(args[0], envp, exit_status);
	if (!full_path)
		return ;

	if (stat(full_path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		is_directory(full_path, exit_status);
		free(full_path);
		return ;
	}

	// Pass is_background flag and last_bg_pid to execute_process
	execute_process(full_path, args, envp, exit_status, is_background, last_bg_pid);

	if (full_path != args[0])
		free(full_path);
}

