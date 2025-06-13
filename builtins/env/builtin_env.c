/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 14:10:50 by carlos-j          #+#    #+#             */
/*   Updated: 2025/06/13 15:41:38 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	env_has_error(char **args, t_shell *shell)
{
	if (!shell || !shell->envp)
	{
		ft_putstr_fd("env: environment not set\n", 2);
		if (shell)
			shell->exit_status = 1;
		return (1);
	}
	if (args[1] && !is_redirection_operator(args[1])
		&& ft_strncmp(args[1], "|", 2) != 0)
	{
		ft_putstr_fd("env: too many arguments\n", 2);
		shell->exit_status = 1;
		return (1);
	}
	return (0);
}

int	builtin_env(char **args, t_shell *shell)
{
	int		i;
	char	*equal_sign;

	if (env_has_error(args, shell))
		return (1);
	i = 0;
	while (shell->envp[i])
	{
		equal_sign = ft_strchr(shell->envp[i], '=');
		if (equal_sign && equal_sign[1] != '\0')
			printf("%s\n", shell->envp[i]);
		i++;
	}
	shell->exit_status = 0;
	return (0);
}
