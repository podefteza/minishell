/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:11:46 by carlos-j          #+#    #+#             */
/*   Updated: 2025/06/13 08:20:53 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_signal(int sig, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_signal_status = SIGINT;
	}
}

void	setup_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = NULL;
	sa.sa_sigaction = &handle_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
}
