/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 16:59:49 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/03 17:01:30 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

char	**handle_new_export_entry(char **export_list, char *new_entry)
{
	char	**new_list;
	int		count;
	int		i;

	count = 0;
	i = 0;
	while (export_list && export_list[count])
		count++;
	new_list = malloc((count + 2) * sizeof(char *));
	if (!new_list)
	{
		free(new_entry);
		return (export_list);
	}
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

char	**add_or_update_export_list(char **export_list, const char *key,
		const char *value)
{
	int		index;
	char	*new_entry;

	index = find_export_var(export_list, key);
	if (index != -1)
	{
		if (!value && ft_strchr(export_list[index], '='))
			return (export_list);
		new_entry = create_export_entry(key, value);
		if (!new_entry)
			return (export_list);
		free(export_list[index]);
		export_list[index] = new_entry;
		return (export_list);
	}
	new_entry = create_export_entry(key, value);
	if (!new_entry)
		return (export_list);
	return (handle_new_export_entry(export_list, new_entry));
}
