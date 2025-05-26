/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 13:55:32 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/26 16:45:08 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	check_for_pipe(t_shell *shell)
{
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
	int		stdin_backup;
	int		stdout_backup;
	int		need_restore;

	stdin_backup = -1;
	stdout_backup = -1;
	need_restore = FALSE;
	if (shell->input.commands && shell->input.commands[0])
	{
		stdin_backup = dup(STDIN_FILENO);
		stdout_backup = dup(STDOUT_FILENO);
		need_restore = TRUE;
	}
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
	if (!shell->input.commands || !shell->input.commands[0])
	{
		if (need_restore)
		{
			close(stdin_backup);
			close(stdout_backup);
		}
		return ;
	}
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
	execute_command(shell);
	if (need_restore)
		restore_stdio(stdout_backup, stdin_backup);
}

void	handle_input(t_shell *shell)
{
	handle_signal_status(shell);
	process_initial_input(shell);
	if (!shell->input.processed)
	{
		return ;
	}
	check_for_expansion(shell);
	if (!shell->input.expanded || shell->input.expanded[0] == '\0')
	{
		free(shell->input.expanded);
		return ;
	}
	if (validate_syntax(shell))
	{
		free(shell->input.expanded);
		return ;
	}
	check_for_pipe(shell);
	split_commands(shell);
	if (remove_quotes_from_commands(shell))
	{
		free_input(shell);
		return ;
	}
	execute_final_command(shell);
	free_input(shell);
}
