/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 13:55:32 by carlos-j          #+#    #+#             */
/*   Updated: 2025/06/15 19:01:42 by carlos-j         ###   ########.fr       */
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

static int	preprocess_and_expand(t_shell *shell)
{
	char	*processed_input;

	processed_input = preprocess_heredocs(shell->input.processed, shell);
	if (!processed_input || g_signal_status)
	{
		free(processed_input);
		cleanup_all_temp_files(shell);
		close_all_fds();
		free(shell->input.processed);
		shell->exit_status = 130;
		g_signal_status = 0;
		return (1);
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
		return (1);
	}
	return (0);
}

static int	validate_and_prepare_commands(t_shell *shell)
{
	if (validate_syntax(shell))
	{
		cleanup_all_temp_files(shell);
		close_all_fds();
		free(shell->input.expanded);
		return (1);
	}
	check_for_pipe(shell);
	split_commands(shell);
	if (remove_quotes_from_commands(shell))
	{
		cleanup_all_temp_files(shell);
		free_input(shell);
		close_all_fds();
		return (1);
	}
	return (0);
}

void	handle_input(t_shell *shell)
{
	if (g_signal_status)
	{
		shell->exit_status = 130;
		g_signal_status = 0;
	}
	if (process_initial_input(shell))
		return ;
	if (!shell->input.processed)
		return ;
	if (preprocess_and_expand(shell))
		return ;
	if (validate_and_prepare_commands(shell))
		return ;
	execute_final_command(shell);
	cleanup_all_temp_files(shell);
	free_input(shell);
	close_all_fds();
}
