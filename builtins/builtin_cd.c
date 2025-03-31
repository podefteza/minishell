/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:00:20 by carlos-j          #+#    #+#             */
/*   Updated: 2025/03/28 14:18:49 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_cd_error(t_shell *shell)
{
	shell->exit_status = 1;
	ft_putstr_fd("minishell: cd: too many arguments\n", 2);
}

void	change_to_home_directory(t_shell *shell)
{
	char	*home;

	home = getenv("HOME");
	if (home)
	{
		if (chdir(home) == 0)
			shell->exit_status = 0;
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

char	*expand_tilde(char *path)
{
	char	*home;
	char	*expanded_path;

	home = getenv("HOME");
	if (home)
	{
		expanded_path = malloc(ft_strlen(home) + ft_strlen(path));
		if (expanded_path)
		{
			ft_strlcpy(expanded_path, home, ft_strlen(home) + 1);
			ft_strlcat(expanded_path, path + 1, ft_strlen(expanded_path)
				+ ft_strlen(path));
			return (expanded_path);
		}
	}
	return (NULL);
}

void	change_directory(char **args, t_shell *shell)
{
	if (chdir(args[1]) == 0)
		shell->exit_status = 0;
	else
	{
		shell->exit_status = 1;
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": ", 2);
		perror("");
	}
}

int	builtin_cd(char **args, t_shell *shell)
{
	char	*expanded_path;

	if (args[1] && args[2])
	{
		handle_cd_error(shell);
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
	else
		change_directory(args, shell);

	return (shell->exit_status);
}

