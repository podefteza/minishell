/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handler_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 17:29:00 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/21 14:50:02 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	command_not_found(char **args, t_shell *shell)
{
	ft_puterr("minishell: ", args[0], CNF, "\n");
	shell->exit_status = 127;
}

char	*find_command(char *cmd, t_shell *shell)
{
	char	*full_path;
	char	*path;
	int		status;

	full_path = NULL;
	status = cmd_is_path(cmd, &full_path, shell);
	if (status == 126 || status == 127)
		return (NULL);
	if (status == 0 && full_path)
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

int	count_executables(char ***commands)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (commands[i])
	{
		if (commands[i][0] && ft_strncmp(commands[i][0], "|", 2) != 0)
			count++;
		i++;
	}
	return (count);
}

void	safe_close(int fd)
{
	if (fd != -1)
		close(fd);
}
