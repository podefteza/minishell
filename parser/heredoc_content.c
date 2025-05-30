/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_content.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 11:10:34 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/30 10:59:39 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	heredoc_sigint_handler(int signo)
{
	(void)signo;
	g_signal_status = 1;
	write(STDERR_FILENO, "\n", 1);
}

char	*collect_heredoc_content(char *delimiter, int expand, t_shell *shell)
{
	char				*content;
	struct sigaction	sa;
	struct sigaction	old_sa;
	int					result;

	content = ft_strdup("");
	if (!content)
		return (NULL);
	sa.sa_handler = heredoc_sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, &old_sa);
	g_signal_status = 0;
	while (!g_signal_status)
	{
		result = process_single_heredoc_line(&content, delimiter, expand,
				shell);
		if (result <= 0)
			break ;
	}
	sigaction(SIGINT, &old_sa, NULL);
	return (content);
}
