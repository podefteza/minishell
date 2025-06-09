/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 02:04:37 by carlos-j          #+#    #+#             */
/*   Updated: 2025/06/09 11:55:47 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_inside_quotes(const char *str, const char *pos)
{
	const char	*p = str;
	int			in_single;
	int			in_double;

	in_single = 0;
	in_double = 0;
	while (p < pos)
	{
		if (*p == '\'' && !in_double)
			in_single = !in_single;
		else if (*p == '"' && !in_single)
			in_double = !in_double;
		p++;
	}
	return (in_single || in_double);
}

static int	is_valid_heredoc_delimiter_char(char c)
{
	return (ft_isalnum(c) || c == '_' || c == '-');
}

static int	is_valid_heredoc_start(char *pos)
{
	pos += 2;
	if (*pos == '-')
		pos++;
	while (*pos == ' ' || *pos == '\t')
		pos++;
	if (!*pos || (!is_valid_heredoc_delimiter_char(*pos) && *pos != '\''
			&& *pos != '"'))
		return (0);
	return (1);
}

static char	*find_next_heredoc(char *start, char *pos)
{
	pos = ft_strnstr(pos, "<<", ft_strlen(pos));
	while (pos)
	{
		if (!is_inside_quotes(start, pos))
		{
			if (is_valid_heredoc_start(pos))
				return (pos);
		}
		pos += 2;
		pos = ft_strnstr(pos, "<<", ft_strlen(pos));
	}
	return (NULL);
}

char	*preprocess_heredocs(char *input, t_shell *shell)
{
	char	*result;
	char	*pos;
	int		status;

	result = ft_strdup(input);
	if (!result)
		return (NULL);
	pos = result;
	pos = find_next_heredoc(result, pos);
	while (pos)
	{
		status = handle_heredoc_replacement(&result, &pos, shell);
		if (status == -1)
		{
			free(result);
			return (NULL);
		}
		if (status == 0)
			pos += 2;
		pos = find_next_heredoc(result, pos);
	}
	return (result);
}
