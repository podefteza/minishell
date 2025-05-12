/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:19:12 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/09 23:09:18 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	shell->is_prompting = 1;
	return (readline(prompt));
}

static void	process_input_line(char *input, t_shell *shell)
{
	if (*input)
	{
		add_history(input);
		handle_input(input, shell);
	}
}

void	run_shell_loop(t_shell *shell)
{
	char	cwd[PATH_MAX];
	char	prompt[PROMPT_MAX];
	char	*display_path;
	char	*input;

	while (1)
	{
		display_path = get_display_path(cwd, shell);
		input = read_user_input(shell, display_path, prompt);
		shell->is_prompting = 0;
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		process_input_line(input, shell);

	}
	free_shell_resources(shell);
	clear_history();
}
