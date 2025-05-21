/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:59:45 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/21 14:18:09 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	validate_pwd_args(char **args, t_shell *shell)
{
	int	i;

	i = 0;
	while (args && args[i])
	{
		if (i > 0 && ft_strncmp(args[i], "pwd", 4) != 0)
		{
			if (args[i][0] == '-' && !is_redirection_operator(args[i])
				&& ft_strncmp(args[i], "|", 2) != 0)
			{
				ft_putstr_fd("minishell: pwd: no options supported\n", 2);
				shell->exit_status = 1;
				return (1);
			}
		}
		i++;
	}
	return (0);
}

int	builtin_pwd(char **args, t_shell *shell)
{
	char	cwd[PATH_MAX];

	if (validate_pwd_args(args, shell))
		return (1);
	if (getcwd(cwd, sizeof(cwd)))
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
