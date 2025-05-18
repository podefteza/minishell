/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_child_process.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 14:09:19 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/16 11:18:13 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	has_heredoc(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (ft_strncmp(args[i], "<<", 3) == 0)
			return (1);
		i++;
	}
	return (0);
}

static int setup_child_io(int input_fd, int pipe_fds[2])
{
      if (input_fd != STDIN_FILENO && dup2(input_fd, STDIN_FILENO) == -1)
    {
        perror("minishell: dup2 input");
        return (-1);
    }
    if (pipe_fds[1] != -1 && dup2(pipe_fds[1], STDOUT_FILENO) == -1)
    {
        perror("minishell: dup2 output");
        return (-1);
    }

    // Close all file descriptors we don't need anymore
    if (input_fd != STDIN_FILENO)
        close(input_fd);
    if (pipe_fds[0] != -1)
        close(pipe_fds[0]);
    if (pipe_fds[1] != -1)
        close(pipe_fds[1]);

    return (0);
}

static int	safe_execute_command(char **args, t_shell *shell)
{
	char	*cmd_path;

	//fprintf(stderr, "=========== safe_execute_command\n");

	if (!args || !args[0])
		return (shell->exit_status = 0, 1);
	if (execute_builtins(shell))
	{
		free_array(args); // added this...
		free_shell_resources(shell); // added this...
		exit(shell->exit_status);
	}
	cmd_path = find_command(args[0], shell);
	if (cmd_path)
	{
		execve(cmd_path, args, shell->envp);
		free(cmd_path);
		shell->exit_status = 127;
		//free_array(args); // added this...
		ft_puterr("minishell: ", args[0], CNF, "\n");
	}
	else if (ft_strchr(args[0], '/'))
	{
		if (execve(args[0], args, shell->envp) == -1)
		{
			//free_array(args); // added this...
			shell->exit_status = 127;
			ft_puterr("minishell: ", args[0], CNF, "\n");
		}
	}
	return (0);
}

void	execute_child_process(char **args, int input_fd, int pipe_fds[2],
		t_shell *shell)
{
	int	redir_status;
	int	heredoc_present;

	// print args
	/*for (int i = 0; args[i]; i++)
	{
		fprintf(stderr, "args[%d]: %s\n", i, args[i]);
	}*/

	signal(SIGPIPE, SIG_DFL);
	heredoc_present = has_heredoc(args);
	if (!heredoc_present && (setup_child_io(input_fd, pipe_fds) != 0))
	{
		//close(pipe_fds[0]);
		//printf("debug1\n");
		free_array(args); // added this...
		free_shell_resources(shell); // added this...
		//fprintf(stderr, "=========== end of execute_child_process1\n");
		exit(1);
	}
	//fprintf(stderr, "=========== will execute this\n");
	redir_status = handle_redirections(args, shell);
	if (redir_status != 0 || !args[0])
	{
		//printf("debug2\n");
		free_shell_resources(shell);
		free_array(args); // added this...
		//fprintf(stderr, "=========== end of execute_child_process2\n");
		if (redir_status != 0)
			exit(shell->exit_status);
		else
			exit(0);
	}
	//fprintf(stderr, "=========== will also execute this?????????????\n");
	if (heredoc_present && (setup_child_io(input_fd, pipe_fds) != 0))
	{
		//printf("debug3\n");
		free_array(args); // added this...
		free_shell_resources(shell); // added this...
		//fprintf(stderr, "=========== end of execute_child_process3\n");
		exit(1);
	}
	//printf("debug4\n");
	safe_execute_command(args, shell);
	free_array(args);
	free_shell_resources(shell);
	//fprintf(stderr, "=========== end of execute_child_process4\n");
	exit(shell->exit_status);


}
