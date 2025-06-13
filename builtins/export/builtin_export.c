/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 09:14:15 by carlos-j          #+#    #+#             */
/*   Updated: 2025/06/13 15:06:57 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	handle_export_with_value(char *arg, t_shell *shell, char *copy,
		char *equal_sign)
{
	char	*key;
	char	*value;

	*equal_sign = '\0';
	key = copy;
	value = equal_sign + 1;
	if (!is_valid_identifier(key))
		print_export_error(arg, shell);
	else
	{
		shell->export_list = add_or_update_export_list(shell->export_list, key,
				value);
		shell->envp = add_or_update_env(shell, key, value);
	}
}

static void	handle_export_assignment(char *arg, t_shell *shell)
{
	char	*copy;
	char	*equal_sign;

	copy = ft_strdup(arg);
	if (!copy)
		return ;
	equal_sign = ft_strchr(copy, '=');
	if (equal_sign)
		handle_export_with_value(arg, shell, copy, equal_sign);
	else if (!is_valid_identifier(copy))
		print_export_error(arg, shell);
	else
		shell->export_list = add_or_update_export_list(shell->export_list, copy,
				NULL);
	free(copy);
}

static char	*create_export_display_entry(const char *key, const char *value)
{
	char	*tmp1;
	char	*tmp2;
	char	*entry;

	if (!value)
		return (ft_strdup(key));
	tmp1 = ft_strjoin(key, "=\"");
	if (!tmp1)
		return (NULL);
	tmp2 = ft_strjoin(tmp1, value);
	free(tmp1);
	if (!tmp2)
		return (NULL);
	entry = ft_strjoin(tmp2, "\"");
	free(tmp2);
	return (entry);
}

static void	display_export_entry(char *entry)
{
	char	*equal_sign;
	char	*display_entry;

	equal_sign = ft_strchr(entry, '=');
	if (equal_sign)
	{
		*equal_sign = '\0';
		display_entry = create_export_display_entry(entry, equal_sign + 1);
		*equal_sign = '=';
		if (display_entry)
		{
			printf("declare -x %s\n", display_entry);
			free(display_entry);
		}
	}
	else
		printf("declare -x %s\n", entry);
}

int	builtin_export(char **args, t_shell *shell)
{
	int	i;

	if (!args[1])
	{
		sort_env(shell->export_list);
		i = 0;
		while (shell->export_list[i])
		{
			display_export_entry(shell->export_list[i]);
			i++;
		}
		return (0);
	}
	i = 1;
	while (args[i])
	{
		handle_export_assignment(args[i], shell);
		i++;
	}
	return (shell->exit_status);
}
