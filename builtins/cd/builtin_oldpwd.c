/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_oldpwd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 15:35:55 by carlos-j          #+#    #+#             */
/*   Updated: 2025/06/13 15:34:41 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*get_oldpwd_value(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "OLDPWD=", 7) == 0)
			return (envp[i] + 7);
		i++;
	}
	return (NULL);
}

static void	handle_cd_to_oldpwd(t_shell *shell, char *oldpwd, char *old_pwd)
{
	char	new_pwd[PATH_MAX];

	if (chdir(oldpwd) == 0)
	{
		shell->envp = add_or_update_env(shell, "OLDPWD", old_pwd);
		shell->export_list = add_or_update_export_list(shell->export_list,
				"OLDPWD", old_pwd);
		if (getcwd(new_pwd, sizeof(new_pwd)))
		{
			shell->envp = add_or_update_env(shell, "PWD", new_pwd);
			shell->export_list = add_or_update_export_list(shell->export_list,
					"PWD", new_pwd);
			ft_putendl_fd(new_pwd, STDOUT_FILENO);
		}
		shell->exit_status = 0;
	}
	else
	{
		shell->exit_status = 1;
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(oldpwd, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror("");
	}
}

void	change_to_previous_directory(t_shell *shell)
{
	char	*oldpwd;
	char	old_pwd[PATH_MAX];

	oldpwd = get_oldpwd_value(shell->envp);
	if (!oldpwd || !*oldpwd)
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
		shell->exit_status = 1;
		return ;
	}
	if (!getcwd(old_pwd, sizeof(old_pwd)))
		ft_strlcpy(old_pwd, "", sizeof(old_pwd));
	handle_cd_to_oldpwd(shell, oldpwd, old_pwd);
}
