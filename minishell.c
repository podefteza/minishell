/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:13:07 by carlos-j          #+#    #+#             */
/*   Updated: 2025/03/14 15:40:22 by carlos-j         ###   ########.fr       */
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
}

void	run_shell_loop(t_shell *shell)
{
	char	*input;
	char	cwd[PATH_MAX];
	char	prompt[PROMPT_MAX];
	char	*display_path;

	while (1)
	{
		if (getcwd(cwd, sizeof(cwd)) == NULL)
		{
			perror("getcwd() error");
			ft_strlcpy(cwd, "unknown", sizeof(cwd));
		}
		display_path = shorten_path(cwd, shell->home);
		if (g_signal_status)
		{
			g_signal_status = 0;
			build_prompt(prompt, shell, display_path);
		}
		else
		{
			build_prompt(prompt, shell, display_path);
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
			handle_input(input, shell);
		}
		free(input);
	}
	free_shell_resources(shell);
}

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
	setup_shell(&shell, envp);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handle_signal);
	g_signal_status = 0;
	run_shell_loop(&shell);
	exit_status = shell.exit_status;
	free_shell_resources(&shell);
	return (exit_status);
}
