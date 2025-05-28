/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 12:02:59 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/28 01:08:23 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_builtin(char *cmd, t_shell *shell)
{
	int	i;

	if (!cmd)
		return (FALSE);
	i = 0;
	while (shell->builtins[i].cmd)
	{
		if (ft_strncmp(cmd, shell->builtins[i].cmd, ft_strlen(cmd)) == 0)
			return (TRUE);
		i++;
	}
	return (FALSE);
}

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

static void	handle_child_execution(char **args, t_shell *shell)
{
	char	*full_path;
	int i;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	full_path = validate_and_find_command(args, shell);
	if (!full_path)
	{
		free_input(shell);
		free_shell_resources(shell);
		i = 3;
		while (i < 1024)
		{
			close(i);
			i++;
		}
		exit(shell->exit_status);
	}
	execve(full_path, args, shell->envp);
	perror("execve");
	if (full_path != args[0])
		free(full_path);
	shell->exit_status = 126;
	free_shell_resources(shell);
	close_all_fds();
	exit(shell->exit_status);
}

void	execute_child(t_shell *shell, char **args, int prev, int pipe_fd[2])
{
	//printf(">>>>>>>>>>> execute_child\n");

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (prev != -1)
	{
		dup2(prev, STDIN_FILENO);
		close(prev);
	}
	if (pipe_fd[1] != -1)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
	safe_close(pipe_fd[0]);
	if (handle_redirections(args, shell) == -1)
	{
		free_shell_resources(shell);  // OK!
		free_input(shell); // OK!
		close_all_fds();
		exit(1);
	}
	if (is_builtin(args[0], shell))
	{
		execute_builtins(shell, args);
		free_shell_resources(shell);
		free_input(shell); // OK!
		close_all_fds();
		exit(shell->exit_status);
	}
	safe_close(prev);
	handle_child_execution(args, shell);
}
