/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_content_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 10:42:09 by carlos-j          #+#    #+#             */
/*   Updated: 2025/06/16 10:04:17 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*process_heredoc_line(char *line, int expand, t_shell *shell)
{
	char	*processed_line;

	if (!expand)
		return (line);
	processed_line = expand_variables(line, shell);
	free(line);
	return (processed_line);
}

static void	cleanup_heredoc_content(char **content, char **line)
{
	if (content && *content)
	{
		free(*content);
		*content = NULL;
	}
	if (line && *line)
	{
		free(*line);
		*line = NULL;
	}
}

static int	handle_line_input(char **line, char *delimiter)
{
	size_t	len;

	*line = readline("> ");
	if (!*line || g_signal_status)
	{
		if (*line)
			free(*line);
		*line = NULL;
		return (0);
	}
	if (!delimiter)
		return (1);
	len = ft_strlen(delimiter);
	if (ft_strncmp(*line, delimiter, len) == 0 && ft_strlen(*line) == len)
	{
		free(*line);
		*line = NULL;
		return (0);
	}
	return (1);
}

static int	update_heredoc_content(char **content, char *processed_line)
{
	char	*temp;
	char	*new_content;

	temp = ft_strjoin(*content, processed_line);
	free(*content);
	if (!temp)
	{
		*content = NULL;
		return (0);
	}
	new_content = ft_strjoin(temp, "\n");
	free(temp);
	if (!new_content)
	{
		*content = NULL;
		return (0);
	}
	*content = new_content;
	return (1);
}

int	process_single_heredoc_line(char **content, char *delimiter, int expand,
		t_shell *shell)
{
	char	*line;
	char	*processed_line;
	int		result;
	int		success;

	result = handle_line_input(&line, delimiter);
	if (result <= 0)
		return (result);
	processed_line = process_heredoc_line(line, expand, shell);
	if (!processed_line)
	{
		cleanup_heredoc_content(content, NULL);
		return (-1);
	}
	success = update_heredoc_content(content, processed_line);
	free(processed_line);
	if (success == 1)
		return (1);
	return (-1);
}
