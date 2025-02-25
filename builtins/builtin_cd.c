/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:00:20 by carlos-j          #+#    #+#             */
/*   Updated: 2025/02/24 11:37:28 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	change_directory(char **args)
{
	if (chdir(args[1]) == 0)
		g_exit_status = 0;
	else
	{
		g_exit_status = 1;
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": ", 2);
		perror("");
	}
}

void	builtin_cd(char **args)
{
	char	*home;

	if (!args[1])
	{
		home = getenv("HOME");
		if (home)
		{
			if (chdir(home) == 0)
				g_exit_status = 0;
			else
			{
				g_exit_status = 1;
				perror("minishell: cd");
			}
		}
		else
		{
			g_exit_status = 1;
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		}
	}
	else
		change_directory(args);
}
