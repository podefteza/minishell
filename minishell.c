/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:13:07 by carlos-j          #+#    #+#             */
/*   Updated: 2025/03/10 17:02:09 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// ✅ Only one global variable to indicate a received signal is allowed
volatile sig_atomic_t	g_signal_status = 0;

void	handle_signal(int sig)
{
	if (sig == SIGINT)
	{
		g_signal_status = 1;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (sig == SIGQUIT)
		write(1, "\b\b  \b\b", 6);
}

void	run_shell_loop(t_shell *shell, char **envp, int *exit_status)
{
	char	*input;
	char	cwd[PATH_MAX];
	char	prompt[PROMPT_MAX];
	char	*display_path;
	pid_t		last_bg_pid;

	last_bg_pid = -1;
	while (1)
	{
		if (getcwd(cwd, sizeof(cwd)) == NULL)
		{
			perror("getcwd() error");
			ft_strlcpy(cwd, "unknown", sizeof(cwd));
		}
		display_path = shorten_path(cwd, shell->home);
		if (!g_signal_status)
			build_prompt(prompt, shell->user, shell->hostname, display_path);
		else
		{
			g_signal_status = 0;
			prompt[0] = '\0';
		}
		input = readline(prompt);
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
		{
			add_history(input);
			handle_input(input, envp, exit_status, &last_bg_pid);
		}
		free(input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	int		exit_status;

	exit_status = 0;
	(void)argv;
	if (argc != 1)
	{
		ft_putstr_fd("error: minishell should not have any arguments.\n", 2);
		return (1);
	}
	setup_shell(&shell);
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, handle_signal);
	g_signal_status = 0;
	run_shell_loop(&shell, envp, &exit_status);
	return (exit_status);
}
