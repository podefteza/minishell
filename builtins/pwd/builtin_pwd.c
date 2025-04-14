/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:59:45 by carlos-j          #+#    #+#             */
/*   Updated: 2025/04/14 15:18:30 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	builtin_pwd(char **args, t_shell *shell)
{
	char	cwd[PATH_MAX];
	int		i;

	i = 0;
	while (args && args[i])
	{
		if (i > 0 && ft_strncmp(args[i], "pwd", 4) != 0)
		{
			if (args[i][0] == '-' && !is_redirection_operator(args[i])
				&& ft_strncmp(args[i], "|", 2) != 0)
			{
				ft_putstr_fd("minishell: pwd should be executed with no options\n",
					2);
				shell->exit_status = 1;
				return (1);
			}
		}
		i++;
	}
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		shell->exit_status = 0;
		return (0);
	}
	else
	{
		perror("minishell: pwd");
		shell->exit_status = 1;
		return (1);
	}
}
