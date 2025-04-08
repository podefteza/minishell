/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 09:14:15 by carlos-j          #+#    #+#             */
/*   Updated: 2025/04/08 11:23:41 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
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

void	sort_env(char **envp)
{
	int		swapped;
	int		i;
	char	*temp;

	swapped = TRUE;
	while (swapped)
	{
		swapped = FALSE;
		i = 0;
		while (envp[i + 1])
		{
			if (strcmp(envp[i], envp[i + 1]) > 0)
			{
				temp = envp[i];
				envp[i] = envp[i + 1];
				envp[i + 1] = temp;
				swapped = TRUE;
			}
			i++;
		}
	}
}

void	print_export_error(char *arg, t_shell *shell)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	shell->exit_status = 1;
}

void	handle_export_assignment(char *arg, t_shell *shell)
{
	char	*key;
	char	*value;
	char	*equal_sign;

	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		*equal_sign = '\0';
		key = arg;
		value = equal_sign + 1;
		if (!is_valid_identifier(key))
			print_export_error(arg, shell);
		else
			shell->envp = add_or_update_env(shell, key, value);
		*equal_sign = '=';
	}
	else if (is_valid_identifier(arg))
		shell->envp = add_or_update_env(shell, arg, "");
	else
		print_export_error(arg, shell);
}

int	builtin_export(char **args, t_shell *shell)
{
	int	i;

	if (!args[1])
	{
		sort_env(shell->envp);
		i = 0;
		while (shell->envp[i])
			printf("declare -x %s\n", shell->envp[i++]);
		return (0);
	}
	i = 1;
	while (args[i])
		handle_export_assignment(args[i++], shell);

	return (shell->exit_status);
}

