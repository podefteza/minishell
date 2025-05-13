/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_redirection_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 15:26:02 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/12 19:52:18 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*extract_redir_op(const char **ptr)
{
	char	*redir;

	if (**ptr == '<' && *(*ptr + 1) == '<')
		redir = ft_strdup("<<");
	else
		redir = ft_strdup("<");
	*ptr += ft_strlen(redir);
	return (redir);
}

static char	*extract_token(const char **ptr)
{
	const char	*start;
	char		*token;

	while (**ptr && ft_isspace(**ptr))
		(*ptr)++;
	start = *ptr;
	while (**ptr && !ft_isspace(**ptr))
		(*ptr)++;
	token = ft_substr(start, 0, *ptr - start);
	while (**ptr && ft_isspace(**ptr))
		(*ptr)++;
	return (token);
}

static char	*rebuild_command(char *cmd_part, char *redir_op, char *filename)
{
	char	*new_cmd;
	char	*tmp;

	new_cmd = ft_strjoin(cmd_part, " ");
	tmp = ft_strjoin(new_cmd, redir_op);
	free(new_cmd);
	new_cmd = ft_strjoin(tmp, " ");
	free(tmp);
	tmp = ft_strjoin(new_cmd, filename);
	free(new_cmd);
	return (tmp);
}

void	move_redirection(char **commands, int i, const char *cmd_start)
{
	char		*redir_op;
	char		*filename;
	char		*cmd_part;
	char		*new_cmd;
	const char	*ptr = cmd_start;

	redir_op = extract_redir_op(&ptr);
	filename = extract_token(&ptr);
	cmd_part = ft_strdup(ptr);
	if (redir_op && filename && cmd_part)
	{
		new_cmd = rebuild_command(cmd_part, redir_op, filename);
		free(commands[i]);
		commands[i] = new_cmd;
	}
	free(redir_op);
	free(filename);
	free(cmd_part);
}
