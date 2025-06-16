/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils3.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 15:59:37 by carlos-j          #+#    #+#             */
/*   Updated: 2025/06/16 16:04:09 by carlos-j         ###   ########.fr       */
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

void	handle_simple_export(char *arg, t_shell *shell, char *copy)
{
	char	*equal_sign;

	equal_sign = ft_strchr(copy, '=');
	if (equal_sign)
		handle_export_with_value(arg, shell, copy, equal_sign);
	else if (!is_valid_identifier(copy))
		print_export_error(arg, shell);
	else
		shell->export_list = add_or_update_export_list(shell->export_list, copy,
				NULL);
}

static char	*get_old_value_for_append(char *key, t_shell *shell)
{
	char	*old_value;

	old_value = find_env_value(key, shell);
	if (!old_value)
		old_value = ft_strdup("");
	else
		old_value = ft_strdup(old_value);
	return (old_value);
}

static void	process_append_export(char *key, char *value, t_shell *shell)
{
	char	*old_value;
	char	*new_value;

	old_value = get_old_value_for_append(key, shell);
	if (!old_value)
		return ;
	new_value = ft_strjoin(old_value, value);
	free(old_value);
	if (!new_value)
		return ;
	shell->export_list = add_or_update_export_list(shell->export_list, key,
			new_value);
	shell->envp = add_or_update_env(shell, key, new_value);
	free(new_value);
}

void	handle_append_export(char *arg, t_shell *shell, char *copy)
{
	char	*equal_sign;
	char	*key;
	char	*value;

	equal_sign = ft_strchr(copy, '+');
	*equal_sign = '\0';
	key = copy;
	value = equal_sign + 2;
	if (!is_valid_identifier(key))
		print_export_error(arg, shell);
	else
		process_append_export(key, value, shell);
}
