/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:13:07 by carlos-j          #+#    #+#             */
/*   Updated: 2025/06/06 14:03:50 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal_status = 0;

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	int		exit_status;

	(void)argv;
	if (argc != 1)
	{
		ft_putstr_fd("error: minishell should not have any arguments.\n", 2);
		return (1);
	}
	setup_signals();
	setup_shell(&shell, envp);
	run_shell_loop(&shell);
	exit_status = shell.exit_status;
	return (exit_status);
}
