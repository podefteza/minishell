/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:08:02 by carlos-j          #+#    #+#             */
/*   Updated: 2025/06/16 09:05:55 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	trim_spaces(t_shell *shell)
{
	int	start;
	int	end;
	int	i;

	if (!shell->input.raw)
		return ;
	start = 0;
	while (shell->input.raw[start] == ' ' || shell->input.raw[start] == '\t')
		start++;
	end = ft_strlen(shell->input.raw) - 1;
	while (end > start && (shell->input.raw[end] == ' '
			|| shell->input.raw[end] == '\t'))
		end--;
	shell->input.trim_spaces = malloc((end - start + 2) * sizeof(char));
	if (!shell->input.trim_spaces)
		return ;
	i = 0;
	while (start <= end)
		shell->input.trim_spaces[i++] = shell->input.raw[start++];
	shell->input.trim_spaces[i] = '\0';
}

int	process_initial_input(t_shell *shell)
{
	trim_spaces(shell);
	free(shell->input.raw);
	shell->input.raw = NULL;
	if (!shell->input.trim_spaces || count_quotes(shell->input.trim_spaces))
	{
		free(shell->input.trim_spaces);
		shell->input.trim_spaces = NULL;
		return (1);
	}
	shell->input.processed = expand_tilde_unquoted(shell->input.trim_spaces, shell);
	free(shell->input.trim_spaces);
	shell->input.trim_spaces = NULL;
	if (!shell->input.processed)
		return (1);
	return (0);
}

static int	process_command_args(int is_echo, int i, t_shell *shell)
{
	int		j;
	char	*cleaned;

	j = 0;
	while (shell->input.commands[i][j])
	{
		cleaned = remove_quotes_concat(shell->input.commands[i][j]);
		if (!cleaned)
			return (handle_quote_error(shell->input.commands, i, j, 0));
		if (is_echo && j > 0 && is_redirection_operator(cleaned))
		{
			free(cleaned);
			j++;
			continue ;
		}
		free(shell->input.commands[i][j]);
		shell->input.commands[i][j] = cleaned;
		j++;
	}
	return (0);
}

int	remove_quotes_from_commands(t_shell *shell)
{
	int	i;
	int	is_echo;
	int	result;

	i = 0;
	while (shell->input.commands && shell->input.commands[i])
	{
		is_echo = (shell->input.commands[i][0]
				&& ft_strncmp(shell->input.commands[i][0], "echo",
					ft_strlen(shell->input.commands[i][0])) == 0);
		result = process_command_args(is_echo, i, shell);
		if (result != 0)
			return (result);
		i++;
	}
	return (0);
}
