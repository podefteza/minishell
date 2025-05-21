/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 13:55:32 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/21 15:10:09 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	check_for_pipe(t_shell *shell)
{
	if (!ft_strchr(shell->input.processed, '|')
		|| !is_pipe_outside_quotes(shell->input.processed))
	{
		shell->input.args = malloc(2 * sizeof(char *));
		if (!shell->input.args)
			return ;
		shell->input.args[0] = shell->input.processed;
		shell->input.args[1] = NULL;
		return ;
	}
	shell->input.args = split_pipe(shell);
	if (!shell->input.args)
	{
		shell->input.args = malloc(2 * sizeof(char *));
		if (!shell->input.args)
			return ;
		shell->input.args[0] = shell->input.processed;
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

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	if (!shell->input.commands)
		return ;
	if (shell->input.commands[0] && !is_pipeline(shell))
	{
		if (execute_builtins(shell, shell->input.commands[0]))
		{
			restore_stdio(stdout_backup, stdin_backup);
			if (shell->should_exit == 1)
				exit(shell->exit_status);
			return ;
		}
	}
	execute_command(shell);
	restore_stdio(stdout_backup, stdin_backup);
}

void	handle_input(t_shell *shell)
{
	int	cmd_idx;

	handle_signal_status(shell);
	shell->input.processed = process_initial_input(shell->input.raw);
	if (!shell->input.processed)
		return ;
	shell->input.processed = check_for_expansion(shell);
	if (!shell->input.processed || shell->input.processed[0] == '\0')
	{
		free(shell->input.processed);
		return ;
	}
	if (validate_syntax(shell))
	{
		free(shell->input.processed);
		return ;
	}
	check_for_pipe(shell);
	split_commands(shell);
	remove_quotes_from_commands(&shell->input);
	execute_final_command(shell);
	cmd_idx = 0;
	while (shell->input.commands[cmd_idx])
	{
		free(shell->input.commands[cmd_idx]);
		cmd_idx++;
	}
}
