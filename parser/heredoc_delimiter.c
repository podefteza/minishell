/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_delimiter.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:51:51 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/30 11:53:45 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	init_and_validate_info(t_heredoc_info **info, char *start,
		char **delim_start)
{
	*info = malloc(sizeof(t_heredoc_info));
	if (!*info)
		return (0);
	*delim_start = start + 2;
	while (**delim_start && ft_isspace(**delim_start))
		(*delim_start)++;
	if (!**delim_start)
	{
		free(*info);
		return (0);
	}
	return (1);
}

static int	extract_delimiter_fields(t_heredoc_info *info, char *delim_start,
		char *delim_end, int quoted)
{
	if (quoted)
	{
		delim_start++;
		delim_end--;
	}
	if (delim_end - delim_start <= 0)
	{
		free(info);
		return (0);
	}
	info->delimiter = malloc((delim_end - delim_start) + 1);
	if (!info->delimiter)
	{
		free(info);
		return (0);
	}
	ft_strlcpy(info->delimiter, delim_start, (delim_end - delim_start) + 1);
	info->delimiter[delim_end - delim_start] = '\0';
	info->delim_len = delim_end - delim_start;
	info->quoted = quoted;
	if (quoted)
		info->delim_end = delim_end + 1;
	else
		info->delim_end = delim_end;
	return (1);
}

t_heredoc_info	*parse_heredoc_delimiter(char *heredoc_start)
{
	t_heredoc_info	*info;
	char			*delim_start;
	char			*delim_end;
	int				quoted;

	if (!init_and_validate_info(&info, heredoc_start, &delim_start))
		return (NULL);
	delim_end = delim_start;
	while (*delim_end && !ft_isspace(*delim_end) && *delim_end != '|'
		&& *delim_end != '>')
		delim_end++;
	quoted = (*delim_start == '\'' || *delim_start == '"');
	if (!extract_delimiter_fields(info, delim_start, delim_end, quoted))
		return (NULL);
	return (info);
}
