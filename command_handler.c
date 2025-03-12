/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 14:15:03 by carlos-j          #+#    #+#             */
/*   Updated: 2025/03/12 13:28:46 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	command_not_found(char **args, t_shell *shell)
{
	if (shell->exit_status != 42126 && shell->exit_status != 42127)
	{
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": command not found", 2);
		ft_putstr_fd("\n", 2);
		shell->exit_status = 127;
	}
	if (shell->exit_status == 42126)
		shell->exit_status = 126;
	if (shell->exit_status == 42127)
		shell->exit_status = 127;
}

char	*find_command(char *cmd, char **envp, t_shell *shell)
{
	char	*path;
	char	*full_path;

	full_path = cmd_is_path(cmd, shell);
	if (full_path)
		return (full_path);
	path = get_path_from_env(envp);
	if (!path)
	{
		command_not_found(&cmd, shell);
		return (NULL);
	}
	full_path = search_in_path(path, cmd);
	if (!full_path)
	{
		command_not_found(&cmd, shell);
		return (NULL);
	}
	return (full_path);
}

void	is_directory(char *full_path, t_shell *shell)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(full_path, 2);
	ft_putstr_fd(": Is a directory\n", 2);
	shell->exit_status = 126;
}

void	execute_process(char *full_path, char **args, char **envp,
		int is_background, t_shell *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return ;
	}
	if (pid == 0)
	{
		execve(full_path, args, envp);
		perror("execve");
		exit(1);
	}
	else
	{
		if (is_background)
		{
			shell->last_bg_pid = pid;
			printf("[%d] %s started in background\n", pid, args[0]);
		}
		else
		{
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				shell->exit_status = WEXITSTATUS(status);
		}
	}
}

void	execute_command(char **args, char **envp, t_shell *shell)
{
	char		*full_path;
	struct stat	st;
	int			arg_count;
	int			is_background;

	if (!args || !args[0])
		return ;
	arg_count = 0;
	while (args[arg_count])
		arg_count++;
	is_background = 0;
	if (ft_strncmp(args[arg_count - 1], "&", 2) == 0)
	{
		is_background = 1;
		free(args[arg_count - 1]);
		args[arg_count - 1] = NULL;
	}
	full_path = find_command(args[0], envp, shell);
	if (!full_path)
		return ;
	if (stat(full_path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		is_directory(full_path, shell);
		free(full_path);
		return ;
	}
	execute_process(full_path, args, envp, is_background, shell);
	if (full_path != args[0])
		free(full_path);
}
