/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:57:11 by carlos-j          #+#    #+#             */
/*   Updated: 2025/04/07 10:57:34 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	builtin_exit_with_code(char **args)
{
	if (!args[1] && ft_strlen(args[0]) > 4)
		return (handle_exit_in_first_arg(args));
	if (args[1])
		return (handle_exit_in_second_arg(args));
	return (0);
}

int	builtin_exit(char **args, t_shell *shell)
{
	int	exit_code;

	if (!args || !args[0])
		exit_code = 0;
	else
	{
		exit_code = builtin_exit_with_code(args);
		if (exit_code == -1)
		{
			shell->exit_status = 1;
			return (1);
		}
	}
	ft_putstr_fd("exit\n", 1);
	free_array(args);
	exit(exit_code);
	return (exit_code);
}
