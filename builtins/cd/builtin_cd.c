/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:00:20 by carlos-j          #+#    #+#             */
/*   Updated: 2025/06/17 13:46:40 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	change_to_home_directory(t_shell *shell)
{
	char	*home;
	char	old_pwd[PATH_MAX];

	if (!getcwd(old_pwd, sizeof(old_pwd)))
		ft_strlcpy(old_pwd, "", sizeof(old_pwd));
	home = getenv("HOME");
	if (home)
		handle_home_directory(shell, home, old_pwd);
	else
	{
		shell->exit_status = 1;
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
	}
}

static void	change_directory(char **args, t_shell *shell)
{
	char	old_pwd[PATH_MAX];
	char	new_pwd[PATH_MAX];

	if (!getcwd(old_pwd, sizeof(old_pwd)))
		ft_strlcpy(old_pwd, "", sizeof(old_pwd));
	if (chdir(args[1]) == 0)
	{
		shell->envp = add_or_update_env(shell, "OLDPWD", old_pwd);
		shell->export_list = add_or_update_export_list(shell->export_list,
				"OLDPWD", old_pwd);
		if (getcwd(new_pwd, sizeof(new_pwd)))
		{
			shell->envp = add_or_update_env(shell, "PWD", new_pwd);
			shell->export_list = add_or_update_export_list(shell->export_list,
					"PWD", new_pwd);
		}
		shell->exit_status = 0;
	}
	else
	{
		shell->exit_status = 1;
		ft_puterr("minishell: cd: ", args[1], ": ", NULL);
		perror("");
	}
}

static void	cd_expand_and_change(char *arg, t_shell *shell, char *cmd)
{
	char	*expanded_path;

	expanded_path = expand_tilde_unquoted(arg, shell);
	if (!expanded_path)
	{
		shell->exit_status = 1;
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		return ;
	}
	change_directory((char *[]){cmd, expanded_path, NULL}, shell);
	free(expanded_path);
}

static void	handle_cd_argument(char **args, t_shell *shell)
{
	if (!args[1] || !ft_strncmp(args[1], "--", 3))
		cd_expand_and_change("~", shell, args[0]);
	else if (args[1][0] == '~')
		cd_expand_and_change(args[1], shell, args[0]);
	else if (!ft_strncmp(args[1], "-", 2))
		change_to_previous_directory(shell, 1);
	else
		change_directory(args, shell);
}

int	builtin_cd(char **args, t_shell *shell)
{
	if (args[1] && args[2])
	{
		shell->exit_status = 1;
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	if (!args[1])
	{
		change_to_home_directory(shell);
		return (shell->exit_status);
	}
	handle_cd_argument(args, shell);
	return (shell->exit_status);
}
