/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 14:15:03 by carlos-j          #+#    #+#             */
/*   Updated: 2025/04/08 12:01:48 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	command_not_found(char **args, t_shell *shell)
{
	if (shell->exit_status != 42126 && shell->exit_status != 42127)
	{
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(CNF, 2);
		ft_putstr_fd("\n", 2); // check if we can put \n on the header
		shell->exit_status = 127;
	}
	if (shell->exit_status == 42126)
		shell->exit_status = 126;
	if (shell->exit_status == 42127)
		shell->exit_status = 127;
}

char	*find_command(char *cmd, t_shell *shell)
{
	char	*full_path;
	char	*path;

	full_path = cmd_is_path(cmd, shell);
	if (full_path)
		return (full_path);
	path = get_path_from_env(shell);
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
	ft_putstr_fd(IAD, 2);
	shell->exit_status = 126;
}

void	execute_process(char *full_path, char **args, int is_background,
		t_shell *shell)
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
		execve(full_path, args, shell->envp);
		perror("execve");
		shell->exit_status = 1;
		free_shell_resources(shell); // FREE SHELL RESOURCES
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
			else if (WIFSIGNALED(status))
				shell->exit_status = 128 + WTERMSIG(status);
		}
	}
}

void	execute_command(char **args, t_shell *shell)
{
	char		*full_path;
	struct stat	st;
	int			arg_count;
	int			is_background;
	int			original_stdin;
	int			original_stdout;

	if (!args || !args[0])
		return ;
	original_stdin = dup(STDIN_FILENO);
	original_stdout = dup(STDOUT_FILENO);
	if (handle_redirections(args, shell) == -1)
	{
		dup2(original_stdin, STDIN_FILENO);
		dup2(original_stdout, STDOUT_FILENO);
		close(original_stdin);
		close(original_stdout);
		return ;
	}
	arg_count = 0;
	while (args[arg_count])
		arg_count++;
	is_background = 0;
	if (arg_count > 0 && ft_strncmp(args[arg_count - 1], "&", 2) == 0)
	{
		is_background = 1;
		free(args[arg_count - 1]);
		args[arg_count - 1] = NULL;
	}
	if (!args[0] || args[0][0] == '\0')
		return ;
	full_path = find_command(args[0], shell);
	if (!full_path)
	{
		dup2(original_stdin, STDIN_FILENO);
		dup2(original_stdout, STDOUT_FILENO);
		close(original_stdin);
		close(original_stdout);
		return ;
	}
	if (stat(full_path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		is_directory(full_path, shell);
		free(full_path);
		dup2(original_stdin, STDIN_FILENO);
		dup2(original_stdout, STDOUT_FILENO);
		close(original_stdin);
		close(original_stdout);
		return ;
	}
	execute_process(full_path, args, is_background, shell);
	if (full_path != args[0])
		free(full_path);
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdin);
	close(original_stdout);
}
