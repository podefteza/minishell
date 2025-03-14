/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_setup.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 11:43:51 by carlos-j          #+#    #+#             */
/*   Updated: 2025/03/14 14:36:54 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	build_prompt(char *prompt, t_shell *shell, const char *display_path)
{
	size_t	i;

	i = 0;
	i += ft_strlcpy(prompt + i, BOLD GREEN "[minishell]" RESET " ", PROMPT_MAX
			- i);
	i += ft_strlcpy(prompt + i, BOLD BLUE, PROMPT_MAX - i);
	i += ft_strlcpy(prompt + i, shell->user, PROMPT_MAX - i);
	i += ft_strlcpy(prompt + i, "@", PROMPT_MAX - i);
	i += ft_strlcpy(prompt + i, shell->hostname, PROMPT_MAX - i);
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

void	setup_shell(t_shell *shell, char **envp)
{
	int	i = 0, env_count = 0;

	shell->user = getenv("USER");
	if (!shell->user)
		shell->user = "unknown";
	get_host_name(shell->hostname);
	shell->home = getenv("HOME");
	if (!shell->home)
		shell->home = "";
	shell->exit_status = 0;
	shell->last_bg_pid = -1;
	while (envp[env_count])
		env_count++;
	shell->envp = malloc((env_count + 1) * sizeof(char *));
	if (!shell->envp)
		return ;
	i = 0;
	while (i < env_count)
	{
		shell->envp[i] = ft_strdup(envp[i]);
		if (!shell->envp[i])
		{
			while (i-- > 0)
				free(shell->envp[i]);
			free(shell->envp);
			shell->envp = NULL;
			return ;
		}
		i++;
	}
	shell->envp[env_count] = NULL;
}

