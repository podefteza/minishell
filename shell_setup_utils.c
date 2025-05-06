/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_setup_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:41:21 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/06 16:48:31 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	user_hostname(t_shell *shell)
{
	shell->user = getenv("USER");
	if (!shell->user)
		shell->user = "unknown";
	get_host_name(shell->hostname);
	shell->home = getenv("HOME");
	if (!shell->home)
		shell->home = "";
	shell->exit_status = 0;
	shell->last_bg_pid = -1;
}
