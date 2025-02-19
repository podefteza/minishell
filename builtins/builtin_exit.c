/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:57:11 by carlos-j          #+#    #+#             */
/*   Updated: 2025/02/19 13:07:36 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_exit_with_code(char **args)
{
	int i;
	int exit_code;

	i = 0;
	if (args[1][i] == '+' || args[1][i] == '-')
		i++;
	while (args[1][i])
	{
		if (!ft_isdigit(args[1][i]))
		{
			ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
			free_args(args);
			exit(2);
		}
		i++;
	}
	if (args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (-1);
	}
	exit_code = ft_atoi(args[1]) % 256;
	return (exit_code);
}

void	builtin_exit(char **args)
{
	int	exit_code;

	if (args[1])
	{
		exit_code = builtin_exit_with_code(args);
		if (exit_code == -1)
			return;
	}
	else
		exit_code = 0;
	free_args(args);
	exit(exit_code);
}
