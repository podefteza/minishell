/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:11:46 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/26 15:50:40 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_shell	*get_shell_context(t_shell *new_shell)
{
	static t_shell	*shell = NULL;

	if (new_shell)
		shell = new_shell;
	return (shell);
}

void	reset_terminal_settings(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag |= (ECHO | ICANON | ISIG);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	handle_signal(int sig, siginfo_t *info, void *context)
{
	t_shell	*shell;

	(void)info;
	(void)context;
	reset_terminal_settings();
	shell = get_shell_context(NULL);
	if (!shell)
		return ;
	if (sig == SIGINT)
	{
		g_signal_status = 1;
		write(1, "\n", 1);
		if (shell->is_prompting == TRUE)
		{
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
		else
			shell->exit_status = 130;
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
}
