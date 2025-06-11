/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_temp_file.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 10:57:28 by carlos-j          #+#    #+#             */
/*   Updated: 2025/06/11 15:12:23 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*create_temp_filename(void)
{
	static int	counter = 0;
	char		*counter_str;
	char		*filename;

	counter_str = ft_itoa(counter++);
	if (!counter_str)
		return (NULL);
	filename = ft_strjoin("/tmp/heredoc_", counter_str);
	free(counter_str);
	if (!filename)
		return (NULL);
	return (filename);
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
