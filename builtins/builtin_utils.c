/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 17:04:37 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/07 13:51:47 by carlos-j         ###   ########.fr       */
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

int	execute_builtin(char **args, t_shell *shell)
{
	int	i;

	i = 0;
	if (!args || !args[0] || !shell)
		return (0);
	while (shell->builtins[i].cmd != NULL)
	{
		if (shell->builtins[i].cmd && ft_strncmp(args[0],
				shell->builtins[i].cmd, ft_strlen(shell->builtins[i].cmd)
				+ 1) == 0)
		{
			if (shell->builtins[i].func)
			{
				shell->exit_status = shell->builtins[i].func(args, shell);
				return (1);
			}
		}
		i++;
	}
	return (0);
}
