/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:13:07 by carlos-j          #+#    #+#             */
/*   Updated: 2025/04/14 16:38:16 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// ✅ Only one global variable to indicate a received signal is allowed
volatile sig_atomic_t	g_signal_status = 0;

t_shell	*get_shell_context(t_shell *new_shell)
{
	static t_shell	*shell = NULL;

	if (new_shell)
		shell = new_shell;
	return (shell);
}

void	handle_signal(int sig, siginfo_t *info, void *context)
{
	t_shell	*shell;

	(void)info;
	(void)context;
	shell = get_shell_context(NULL);
	if (!shell)
		return ;
	if (sig == SIGINT)
	{
		g_signal_status = 1;
		write(1, "\n", 1);
		if (shell->is_prompting)
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
			shell->exit_status = 130;
			build_prompt(prompt, shell, display_path);
		}
		else
			build_prompt(prompt, shell, display_path);
		shell->is_prompting = 1;
		input = readline(prompt);
		shell->is_prompting = 0;
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
	}
	free_shell_resources(shell);
	clear_history();
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
	get_shell_context(&shell);
	setup_signals();
	run_shell_loop(&shell);
	exit_status = shell.exit_status;
	free_shell_resources(&shell);
	return (exit_status);
}
