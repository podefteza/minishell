/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_temp_file.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 10:57:28 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/30 11:02:23 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*build_temp_path(char *dir, char *pid_str, char *counter_str)
{
	char	*temp_with_pid;
	char	*temp_path;
	char	*final_path;

	temp_with_pid = ft_strjoin(dir, pid_str);
	if (!temp_with_pid)
		return (NULL);
	temp_path = ft_strjoin(temp_with_pid, "_");
	free(temp_with_pid);
	if (!temp_path)
		return (NULL);
	final_path = ft_strjoin(temp_path, counter_str);
	free(temp_path);
	return (final_path);
}

static char	*create_temp_filename(void)
{
	char		*pid_str;
	char		*counter_str;
	static int	counter = 0;
	char		*result;

	pid_str = ft_itoa(getpid());
	counter_str = ft_itoa(counter++);
	if (!pid_str || !counter_str)
	{
		free(pid_str);
		free(counter_str);
		return (NULL);
	}
	result = build_temp_path("/tmp/heredoc_", pid_str, counter_str);
	free(pid_str);
	free(counter_str);
	return (result);
}

static int	write_heredoc_to_file(char *filename, char *content)
{
	int	fd;

	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
		return (0);
	write(fd, content, ft_strlen(content));
	close(fd);
	return (1);
}

char	*handle_heredoc_to_file(char *delimiter, int expand, t_shell *shell)
{
	char	*content;
	char	*filename;

	content = collect_heredoc_content(delimiter, expand, shell);
	if (!content)
		return (NULL);
	filename = create_temp_filename();
	if (!filename || !write_heredoc_to_file(filename, content))
	{
		free(content);
		free(filename);
		return (NULL);
	}
	free(content);
	return (filename);
}
