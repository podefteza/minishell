/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 17:04:37 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/20 11:40:10 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	builtin_setup(t_builtin *builtins)
{
	builtins[0].cmd = "echo";
	builtins[0].func = builtin_echo;
	builtins[1].cmd = "cd";
	builtins[1].func = builtin_cd;
	builtins[2].cmd = "pwd";
	builtins[2].func = builtin_pwd;
	builtins[3].cmd = "export";
	builtins[3].func = builtin_export;
	builtins[4].cmd = "unset";
	builtins[4].func = builtin_unset;
	builtins[5].cmd = "env";
	builtins[5].func = builtin_env;
	builtins[6].cmd = "exit";
	builtins[6].func = builtin_exit;
	builtins[7].cmd = NULL;
	builtins[7].func = NULL;
}

int	execute_builtins(t_shell *shell, char **cmd)
{
	if (!cmd || !cmd[0])
		return (0);
	for (int i = 0; shell->builtins[i].cmd; i++)
	{
		if (ft_strncmp(cmd[0], shell->builtins[i].cmd,
				ft_strlen(shell->builtins[i].cmd) + 1) == 0)
		{
			if (handle_redirections(cmd, shell) == -1)
			{
				shell->exit_status = 1;
				return (1);
			}
			shell->exit_status = shell->builtins[i].func(cmd, shell);
			return (1);
		}
	}
	return (0);
}
