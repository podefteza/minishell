/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 11:42:14 by carlos-j          #+#    #+#             */
/*   Updated: 2025/06/18 11:43:00 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_redirection_only(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (!is_redirection_token(args[i]))
			return (0);
		i++;
	}
	return (1);
}

void	exit_if_invalid_command(char **args, t_shell *shell)
{
	if ((!args[0] || args[0][0] == '\0') && !is_redirection_only(args))
	{
		ft_puterr("minishell: ", "", "command not found", "\n");
		shell->exit_status = 127;
		free_shell_resources(shell);
		free_input(shell);
		close_all_fds();
		cleanup_all_temp_files(shell);
		exit(shell->exit_status);
	}
}
