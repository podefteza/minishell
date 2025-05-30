/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 11:22:45 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/30 14:37:50 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	print_unset_error(char *arg)
{
	ft_putstr_fd("minishell: unset: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || str[0] == '\0' || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (FALSE);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (FALSE);
		i++;
	}
	return (TRUE);
}

static void	process_unset_arg(char *arg, t_shell *shell)
{
	if (!is_valid_identifier(arg))
	{
		shell->exit_status = 0;
		print_unset_error(arg);
	}
	else
	{
		remove_env_var(shell, arg);
		remove_export_var(shell, arg);
	}
}

int	builtin_unset(char **args, t_shell *shell)
{
	int	i;

	if (!args[1])
	{
		shell->exit_status = 0;
		return (0);
	}
	i = 1;
	while (args[i])
	{
		process_unset_arg(args[i], shell);
		i++;
	}
	if (shell->exit_status != 1)
		shell->exit_status = 0;
	return (shell->exit_status);
}
