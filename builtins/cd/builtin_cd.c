/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:00:20 by carlos-j          #+#    #+#             */
/*   Updated: 2025/04/14 17:30:29 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	change_to_home_directory(t_shell *shell)
{
	char	*home;
	char	old_pwd[PATH_MAX];
	char	new_pwd[PATH_MAX];

	if (!getcwd(old_pwd, sizeof(old_pwd)))
		ft_strlcpy(old_pwd, "", sizeof(old_pwd));
	home = getenv("HOME");
	if (home)
	{
		if (chdir(home) == 0)
		{
			if (getcwd(new_pwd, sizeof(new_pwd)))
			{
				shell->envp = add_or_update_env(shell, "OLDPWD", old_pwd);
				shell->envp = add_or_update_env(shell, "PWD", new_pwd);
			}
			shell->exit_status = 0;
		}
		else
		{
			shell->exit_status = 1;
			perror("minishell: cd");
		}
	}
	else
	{
		shell->exit_status = 1;
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
	}
}

void	change_directory(char **args, t_shell *shell)
{
	char	old_pwd[PATH_MAX];
	char	new_pwd[PATH_MAX];

	if (!getcwd(old_pwd, sizeof(old_pwd)))
		ft_strlcpy(old_pwd, "", sizeof(old_pwd));
	if (chdir(args[1]) == 0)
	{
		shell->envp = add_or_update_env(shell, "OLDPWD", old_pwd);
		if (getcwd(new_pwd, sizeof(new_pwd)))
			shell->envp = add_or_update_env(shell, "PWD", new_pwd);
		shell->exit_status = 0;
	}
	else
	{
		shell->exit_status = 1;
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": ", 2);
		perror("");
	}
}

void	change_to_previous_directory(t_shell *shell)
{
	char	*oldpwd;
	char	old_pwd[PATH_MAX];
	char	new_pwd[PATH_MAX];
	int		i;

	oldpwd = NULL;
	i = 0;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], "OLDPWD=", 7) == 0)
		{
			oldpwd = shell->envp[i] + 7;
			break ;
		}
		i++;
	}
	if (!oldpwd)
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
		shell->exit_status = 1;
		return ;
	}
	if (!getcwd(old_pwd, sizeof(old_pwd)))
		ft_strlcpy(old_pwd, "", sizeof(old_pwd));
	if (chdir(oldpwd) == 0)
	{
		shell->envp = add_or_update_env(shell, "OLDPWD", old_pwd);
		if (getcwd(new_pwd, sizeof(new_pwd)))
			shell->envp = add_or_update_env(shell, "PWD", new_pwd);
		shell->exit_status = 0;
	}
	else
	{
		shell->exit_status = 1;
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(oldpwd, 2);
		ft_putstr_fd(": ", 2);
		perror("");
	}
}

int	builtin_cd(char **args, t_shell *shell)
{
	char	*expanded_path;

	if (args[1] && args[2])
	{
		cd_error(shell);
		return (1);
	}
	if (!args[1])
	{
		change_to_home_directory(shell);
		return (shell->exit_status);
	}
	if (args[1][0] == '~')
	{
		expanded_path = expand_tilde(args[1]);
		if (!expanded_path)
		{
			shell->exit_status = 1;
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return (1);
		}
		change_directory((char *[]){args[0], expanded_path, NULL}, shell);
		free(expanded_path);
	}
	else if (args[1] && !ft_strncmp(args[1], "-", 2))
		change_to_previous_directory(shell);
	else
		change_directory(args, shell);
	return (shell->exit_status);
}
