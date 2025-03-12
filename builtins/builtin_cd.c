/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:00:20 by carlos-j          #+#    #+#             */
/*   Updated: 2025/03/12 13:23:16 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	builtin_cd(char **args, t_shell *shell)
{
	char	*home;
	char	*expanded_path;

	if (args[1] && args[2])  // Check if more than one argument is passed
	{
		shell->exit_status = 1;
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return;
	}

	if (!args[1])  // No argument means cd to HOME
	{
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
	else if (args[1][0] == '~')  // Check if the argument starts with '~'
	{
		home = getenv("HOME");
		if (home)
		{
			// Allocate memory for the expanded path and replace '~' with home directory
			expanded_path = malloc(ft_strlen(home) + ft_strlen(args[1]));
			if (expanded_path)
			{
				// Copy the home directory to the expanded path using ft_strlcpy
				ft_strlcpy(expanded_path, home, ft_strlen(home) + 1);  // Copy the home directory
				ft_strlcat(expanded_path, args[1] + 1, ft_strlen(expanded_path) + ft_strlen(args[1]));  // Append the part after '~'
				change_directory((char *[]){args[0], expanded_path, NULL}, shell);
				free(expanded_path);  // Free the expanded path
			}
			else
			{
				shell->exit_status = 1;
				ft_putstr_fd("minishell: cd: memory allocation error\n", 2);
			}
		}
		else
		{
			shell->exit_status = 1;
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		}
	}
	else  // Normal case, with a path
		change_directory(args, shell);
}
