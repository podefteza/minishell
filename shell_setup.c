/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_setup.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 11:43:51 by carlos-j          #+#    #+#             */
/*   Updated: 2025/03/10 15:42:43 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	build_prompt(char *prompt, const char *user, const char *hostname,
		const char *display_path)
{
	size_t	i;

	i = 0;
	i += ft_strlcpy(prompt + i, BOLD GREEN "[minishell]" RESET " ", PROMPT_MAX
			- i);
	i += ft_strlcpy(prompt + i, BOLD BLUE, PROMPT_MAX - i);
	i += ft_strlcpy(prompt + i, user, PROMPT_MAX - i);
	i += ft_strlcpy(prompt + i, "@", PROMPT_MAX - i);
	i += ft_strlcpy(prompt + i, hostname, PROMPT_MAX - i);
	i += ft_strlcpy(prompt + i, RESET ":", PROMPT_MAX - i);
	i += ft_strlcpy(prompt + i, display_path, PROMPT_MAX - i);
	i += ft_strlcpy(prompt + i, GREEN "$" RESET " ", PROMPT_MAX - i);
}

void	get_host_name(char *hostname)
{
	int	fd;
	int	ret;

	fd = open("/etc/hostname", O_RDONLY);
	if (fd < 0)
	{
		ft_strlcpy(hostname, "unknown", HOSTNAME_MAX);
		return ;
	}
	ret = read(fd, hostname, HOSTNAME_MAX - 1);
	close(fd);
	if (ret > 0)
		hostname[ret - 1] = '\0';
	else
		ft_strlcpy(hostname, "unknown", HOSTNAME_MAX);
}

void	setup_shell(t_shell *shell)
{
	shell->user = getenv("USER");
	if (!shell->user)
		shell->user = "unknown";
	get_host_name(shell->hostname);
	shell->home = getenv("HOME");
	if (!shell->home)
		shell->home = "";
}
