/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 09:14:15 by carlos-j          #+#    #+#             */
/*   Updated: 2025/04/30 11:57:59 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

void	sort_env(char **envp)
{
	int		swapped;
	int		i;
	char	*temp;
	size_t	len1;
	size_t	len2;

	swapped = TRUE;
	while (swapped)
	{
		swapped = FALSE;
		i = 0;
		while (envp[i + 1])
		{
			len1 = ft_strlen(envp[i]);
			len2 = ft_strlen(envp[i + 1]);
			if (len1 > len2)
			{
				if (ft_strncmp(envp[i], envp[i + 1], len1) > 0)
				{
					temp = envp[i];
					envp[i] = envp[i + 1];
					envp[i + 1] = temp;
					swapped = TRUE;
				}
			}
			else
			{
				if (ft_strncmp(envp[i], envp[i + 1], len2) > 0)
				{
					temp = envp[i];
					envp[i] = envp[i + 1];
					envp[i + 1] = temp;
					swapped = TRUE;
				}
			}
			i++;
		}
	}
}

char	*create_export_entry(const char *key, const char *value)
{
	char	*tmp;
	char	*entry;

	if (!value)
		return (ft_strdup(key));
	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (NULL);
	entry = ft_strjoin(tmp, value);
	free(tmp);
	return (entry);
}

char	*create_export_display_entry(const char *key, const char *value)
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

int	find_export_var(char **export_list, const char *key)
{
	int		i;
	size_t	key_len;

	if (!export_list || !key)
		return (-1);
	key_len = ft_strlen(key);
	i = 0;
	while (export_list[i])
	{
		if (ft_strncmp(export_list[i], key, key_len) == 0
			&& (export_list[i][key_len] == '='
				|| export_list[i][key_len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

char	**add_or_update_export_list(char **export_list, const char *key,
		const char *value)
{
	int		existing_index;
	char	*new_entry;
	char	**new_list;
	int		count;
	int		i;

	existing_index = find_export_var(export_list, key);
	if (existing_index != -1 && !value && ft_strchr(export_list[existing_index],
			'='))
		return (export_list);
	new_entry = create_export_entry(key, value);
	if (!new_entry)
		return (export_list);
	if (existing_index != -1)
	{
		free(export_list[existing_index]);
		export_list[existing_index] = new_entry;
		return (export_list);
	}
	count = 0;
	while (export_list && export_list[count])
		count++;
	new_list = malloc((count + 2) * sizeof(char *));
	if (!new_list)
	{
		free(new_entry);
		return (export_list);
	}
	i = 0;
	while (i < count)
	{
		new_list[i] = export_list[i];
		i++;
	}
	new_list[i++] = new_entry;
	new_list[i] = NULL;
	free(export_list);
	return (new_list);
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
	char	*copy;
	char	*key;
	char	*value;
	char	*equal_sign;

	copy = ft_strdup(arg);
	if (!copy)
		return ;
	equal_sign = ft_strchr(copy, '=');
	if (equal_sign)
	{
		*equal_sign = '\0';
		key = copy;
		value = equal_sign + 1;
		if (!is_valid_identifier(key))
			print_export_error(arg, shell);
		else
		{
			shell->export_list = add_or_update_export_list(shell->export_list,
					key, value);
			shell->envp = add_or_update_env(shell, key, value);
		}
	}
	else if (is_valid_identifier(copy))
		shell->export_list = add_or_update_export_list(shell->export_list, copy,
				NULL);
	else
		print_export_error(arg, shell);
	free(copy);
}

int	builtin_export(char **args, t_shell *shell)
{
	int		i;
	char	*equal_sign;
	char	*display_entry;

	if (!args[1])
	{
		sort_env(shell->export_list);
		i = 0;
		while (shell->export_list[i])
		{
			equal_sign = ft_strchr(shell->export_list[i], '=');
			if (equal_sign)
			{
				*equal_sign = '\0';
				display_entry = create_export_display_entry(shell->export_list[i],
						equal_sign + 1);
				*equal_sign = '=';
				if (display_entry)
				{
					printf("declare -x %s\n", display_entry);
					free(display_entry);
				}
			}
			else
				printf("declare -x %s\n", shell->export_list[i]);
			i++;
		}
		return (0);
	}
	i = 1;
	while (args[i])
		handle_export_assignment(args[i++], shell);
	return (shell->exit_status);
}
