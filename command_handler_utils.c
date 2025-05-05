/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handler_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 17:29:00 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/05 11:42:48 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	command_not_found(char **args, t_shell *shell)
{
	ft_puterr("minishell: ", args[0], CNF, "\n");
	shell->exit_status = 127;
}

char	*find_command(char *cmd, t_shell *shell)
{
	char	*full_path;
	char	*path;

	full_path = cmd_is_path(cmd, shell);
	if (shell->exit_status == 126 || shell->exit_status == 127)
		return (NULL);
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
	ft_puterr("minishell: ", full_path, IAD, "\n");
	shell->exit_status = 126;
}
