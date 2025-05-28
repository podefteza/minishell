/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 13:55:32 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/28 13:45:39 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	check_for_pipe(t_shell *shell)
{
	// printf(">>>>>>>>>>> check_for_pipe\n");
	if (!ft_strchr(shell->input.expanded, '|')
		|| !is_pipe_outside_quotes(shell->input.expanded))
	{
		shell->input.args = malloc(2 * sizeof(char *));
		if (!shell->input.args)
			return ;
		shell->input.args[0] = ft_strdup(shell->input.expanded);
		shell->input.args[1] = NULL;
		return ;
	}
	shell->input.args = split_pipe(shell);
	if (!shell->input.args)
	{
		shell->input.args = malloc(2 * sizeof(char *));
		if (!shell->input.args)
			return ;
		shell->input.args[0] = ft_strdup(shell->input.expanded);
		shell->input.args[1] = NULL;
	}
}

static int	is_pipeline(t_shell *shell)
{
	int	i;

	if (!shell->input.commands)
		return (FALSE);
	i = 0;
	while (shell->input.commands[i])
	{
		if (shell->input.commands[i][0]
			&& ft_strncmp(shell->input.commands[i][0], "|", 2) == 0)
		{
			return (TRUE);
		}
		i++;
	}
	return (FALSE);
}

static void	execute_final_command(t_shell *shell)
{
	int	stdin_backup;
	int	stdout_backup;
	int	need_restore;

	//printf(">>>>>>>>>>> execute_final_command\n");
	stdin_backup = -1;
	stdout_backup = -1;
	need_restore = FALSE;
	//printf(">>>> debug1 >>>>>>\n");
	if (shell->input.commands && shell->input.commands[0])
	{
		stdin_backup = dup(STDIN_FILENO);
		stdout_backup = dup(STDOUT_FILENO);
		need_restore = TRUE;
	}
	//printf(">>>> debug2 >>>>>>\n");
	if (shell->input.commands && shell->input.commands[0]
		&& ft_strncmp(shell->input.commands[0][0], "exit", 5) == 0)
	{
		if (need_restore)
		{
			close(stdin_backup);
			close(stdout_backup);
		}
		builtin_exit(shell->input.commands[0], shell);
		return ;
	}
	//printf(">>>> debug3 >>>>>>\n");
	if (!shell->input.commands || !shell->input.commands[0])
	{
		if (need_restore)
		{
			close(stdin_backup);
			close(stdout_backup);
		}
		return ;
	}
	//printf(">>>> debug4 >>>>>>\n");
	if (!is_pipeline(shell))
	{
		if (execute_builtins(shell, shell->input.commands[0]))
		{
			if (shell->should_exit)
			{
				if (need_restore)
				{
					close(stdin_backup);
					close(stdout_backup);
				}
				builtin_exit(shell->input.commands[0], shell);
			}
			else if (need_restore)
				restore_stdio(stdout_backup, stdin_backup);
			return ;
		}
	}
	//printf(">>>> debug5 >>>>>>\n");
	execute_command(shell);
	if (need_restore)
		restore_stdio(stdout_backup, stdin_backup);
	//printf(">>>> debug6 >>>>>>\n");
}

void close_all_fds()
{
	// close all open file descriptors
	//printf("Closing all file descriptors...\n");
	int i = 3;
	while (i < 1024)
		close(i++);
}

void	handle_input(t_shell *shell)
{
	char	*processed_input;

	handle_signal_status(shell);
	process_initial_input(shell);
	if (!shell->input.processed)
		return ;
	processed_input = preprocess_heredocs(shell->input.processed, shell);
	if (!processed_input)
	{
		cleanup_all_temp_files(shell);
		close_all_fds();
		free(shell->input.processed);
		return ;
	}
	free(shell->input.processed);
	shell->input.processed = processed_input;
	check_for_expansion(shell);
	if (!shell->input.expanded || shell->input.expanded[0] == '\0')
	{
		cleanup_all_temp_files(shell);
		close_all_fds();
		shell->exit_status = 0;
		free(shell->input.expanded);
		return ;
	}
	if (validate_syntax(shell))
	{
		cleanup_all_temp_files(shell);
		close_all_fds();
		free(shell->input.expanded);
		return ;
	}
	check_for_pipe(shell);
	split_commands(shell);
	if (remove_quotes_from_commands(shell))
	{
		cleanup_all_temp_files(shell);
		free_input(shell);
		close_all_fds();
		return ;
	}
	//printf("before executing commands...\n");
	execute_final_command(shell);
	cleanup_all_temp_files(shell);
	free_input(shell);
	//printf("end of execution, will close fds\n");
	close_all_fds();
}
