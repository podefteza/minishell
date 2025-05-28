/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:19:12 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/28 00:55:47 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*get_display_path(char *cwd, t_shell *shell)
{
	if (getcwd(cwd, PATH_MAX) == NULL)
	{
		perror("getcwd() error");
		ft_strlcpy(cwd, "unknown", PATH_MAX);
	}
	return (shorten_path(cwd, shell->home));
}

static char	*read_user_input(t_shell *shell, char *display_path, char *prompt)
{
	if (g_signal_status)
	{
		g_signal_status = 0;
		shell->exit_status = 130;
	}
	build_prompt(prompt, shell, display_path);
	shell->is_prompting = TRUE;
	return (readline(prompt));
}

static void	process_input_line(t_shell *shell)
{
	if (*shell->input.raw)
	{
		add_history(shell->input.raw);
		handle_input(shell);
	}
	else
	{
			//free_input(&shell->input);
	}
}

void	run_shell_loop(t_shell *shell)
{
	char	cwd[PATH_MAX];
	char	prompt[PROMPT_MAX];
	char	*display_path;

	while (1)
	{
		display_path = get_display_path(cwd, shell);
		shell->input.raw = read_user_input(shell, display_path, prompt);
		shell->is_prompting = FALSE;
		if (!shell->input.raw)
		{
			shell->should_exit = 1;
			free_shell_resources(shell);
			exit(shell->exit_status);
			//builtin_exit(NULL, shell);
		}
		else
			process_input_line(shell);
		if (shell->should_exit)
			break ;
	}
	free_shell_resources(shell);
	rl_clear_history();
}
