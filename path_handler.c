/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 14:13:36 by carlos-j          #+#    #+#             */
/*   Updated: 2025/04/19 11:10:13 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*shorten_path(const char *cwd, const char *home)
{
	static char	shortened_path[PATH_MAX];
	size_t		home_len;
	size_t		i;

	home_len = ft_strlen(home);
	if (ft_strncmp(cwd, home, home_len) == 0 && cwd[home_len] == '/')
	{
		shortened_path[0] = '~';
		i = 1;
		while (cwd[home_len])
		{
			shortened_path[i] = cwd[home_len];
			i++;
			home_len++;
		}
		shortened_path[i] = '\0';
	}
	else
	{
		i = 0;
		while (cwd[i] && i < PATH_MAX - 1)
		{
			shortened_path[i] = cwd[i];
			i++;
		}
		shortened_path[i] = '\0';
	}
	return (shortened_path);
}

char	*cmd_is_path(char *cmd, t_shell *shell)
{
	if (!ft_strchr(cmd, '/'))
		return (NULL);
	if (access(cmd, F_OK))
	{
		ft_puterr("minishell: ", cmd, NFD, "\n");
		shell->exit_status = 127;
		return (NULL);
	}
	if (access(cmd, X_OK))
	{
		ft_puterr("minishell: ", cmd, PND, "\n");
		shell->exit_status = 126;
		return (NULL);
	}
	return (ft_strdup(cmd));
}

char	*build_path(char *dir, char *cmd)
{
	char	*full_path;
	size_t	total_len;

	total_len = ft_strlen(dir) + ft_strlen(cmd) + 2;
	full_path = malloc(total_len);
	if (!full_path)
		return (NULL);
	full_path[0] = '\0';
	ft_strlcpy(full_path, dir, total_len);
	ft_strlcat(full_path, "/", total_len);
	ft_strlcat(full_path, cmd, total_len);
	if (access(full_path, X_OK) == 0)
		return (full_path);
	free(full_path);
	return (NULL);
}


char	*get_path_from_env(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], "PATH=", 5) == 0)
			return (shell->envp[i] + 5);
		i++;
	}
	return (NULL);
}

char	*search_in_path(char *path, char *cmd)
{
	char	dir[1024];
	int		j;
	char	*full_path;

	j = 0;
	while (*path)
	{
		if (*path == ':')
		{
			dir[j] = '\0';
			full_path = build_path(dir, cmd);
			if (full_path)
				return (full_path);
			j = 0;
		}
		else
		{
			dir[j++] = *path;
		}
		path++;
	}
	dir[j] = '\0';
	return (build_path(dir, cmd));
}
