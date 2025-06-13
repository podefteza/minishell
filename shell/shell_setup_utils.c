/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_setup_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:41:21 by carlos-j          #+#    #+#             */
/*   Updated: 2025/06/13 15:44:26 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	get_host_name(char *hostname)
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
}

void	update_shlvl(t_shell *shell)
{
	char	*lvl_str;
	int		shlvl;
	char	*new_value;

	lvl_str = getenv("SHLVL");
	if (!lvl_str)
		shlvl = 1;
	else
		shlvl = ft_atoi(lvl_str) + 1;
	new_value = ft_itoa(shlvl);
	if (!new_value)
		return ;
	shell->envp = add_or_update_env(shell, "SHLVL", new_value);
	shell->export_list = add_or_update_export_list(shell->export_list, "SHLVL",
			new_value);
	free(new_value);
}

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
	ft_strlcpy(prompt + i, GREEN "$" RESET " ", PROMPT_MAX - i);
}
