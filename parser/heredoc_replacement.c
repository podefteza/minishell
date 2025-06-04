/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_replacement.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:54:39 by carlos-j          #+#    #+#             */
/*   Updated: 2025/06/04 10:41:00 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*create_redirect_replacement(char *delimiter, int quoted,
		t_shell *shell)
{
	char	*temp_filename;
	char	*redirect_and_file;

	temp_filename = handle_heredoc_to_file(delimiter, !quoted, shell);
	if (!temp_filename)
		return (NULL);
	ft_lstadd_back(&shell->temp_files, ft_lstnew(temp_filename));
	redirect_and_file = ft_strjoin("< ", temp_filename);
	return (redirect_and_file);
}

static char	*replace_heredoc_in_string(char *result, char *heredoc_pos,
		char *delim_end, char *replacement)
{
	int		original_len;
	int		replacement_len;
	int		result_len;
	char	*new_result;
	int		pos_offset;

	original_len = delim_end - heredoc_pos;
	replacement_len = ft_strlen(replacement);
	result_len = ft_strlen(result);
	pos_offset = heredoc_pos - result;
	new_result = malloc(result_len - original_len + replacement_len + 1);
	if (!new_result)
		return (NULL);
	ft_strlcpy(new_result, result, pos_offset + 1);
	new_result[pos_offset] = '\0';
	ft_strlcat(new_result, replacement, result_len - original_len
		+ replacement_len + 1);
	ft_strlcat(new_result, delim_end, result_len - original_len
		+ replacement_len + 1);
	return (new_result);
}

static int	apply_replacement(char **result, char **pos, t_heredoc_info *info,
		char *replacement)
{
	char	*new_result;
	int		replacement_len;

	new_result = replace_heredoc_in_string(*result, *pos, info->delim_end,
			replacement);
	if (!new_result)
		return (-1);
	replacement_len = ft_strlen(replacement);
	*pos = new_result + (*pos - *result) + replacement_len;
	free(*result);
	*result = new_result;
	return (1);
}

static void	free_heredoc_info(t_heredoc_info *info)
{
	if (info)
	{
		if (info->delimiter)
			free(info->delimiter);
		free(info);
	}
}

int	handle_heredoc_replacement(char **result, char **pos,
		t_shell *shell)
{
	t_heredoc_info	*info;
	char			*replacement;
	int				status;

	info = parse_heredoc_delimiter(*pos);
	if (!info)
		return (0);
	replacement = create_redirect_replacement(info->delimiter, info->quoted,
			shell);
	if (!replacement)
	{
		free_heredoc_info(info);
		return (-1);
	}
	status = apply_replacement(result, pos, info, replacement);
	free(replacement);
	free_heredoc_info(info);
	return (status);
}
