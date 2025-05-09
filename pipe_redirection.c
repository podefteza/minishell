/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 15:21:40 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/09 18:59:18 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_redirection_only_command(const char *cmd)
{
	while (*cmd && ft_isspace(*cmd))
		cmd++;
	if ((*cmd == '<' || *cmd == '>') && (*(cmd + 1) == ' ' || ((*cmd == '<'
					|| *cmd == '>') && *(cmd + 1) == *cmd && *(cmd
					+ 2) == ' ')))
	{
		while (*cmd && !ft_isspace(*cmd))
			cmd++;
		while (*cmd && ft_isspace(*cmd))
			cmd++;
		while (*cmd && !ft_isspace(*cmd))
			cmd++;
		while (*cmd && ft_isspace(*cmd))
			cmd++;
		return (*cmd == '\0');
	}
	return (0);
}

static void	append_redirection(char **dest, const char *redir, const char *file)
{
	char	*new_cmd;
	char	*tmp;

	tmp = ft_strjoin(redir, " ");
	if (!tmp)
		return ;
	new_cmd = ft_strjoin(tmp, file);
	free(tmp);
	if (!new_cmd)
		return ;
	tmp = ft_strjoin(*dest, " ");
	if (!tmp)
	{
		free(new_cmd);
		return ;
	}
	free(*dest);
	*dest = ft_strjoin(tmp, new_cmd);
	free(tmp);
	free(new_cmd);
}

static void	parse_redir_target(const char *cmd_start, char **redir_op,
		char **filename)
{
	const char	*ptr;
	const char	*start;

	ptr = cmd_start;
	if (*ptr == '<' && *(ptr + 1) == '<')
		*redir_op = ft_strdup("<<");
	else
		*redir_op = ft_strdup("<");
	ptr += ft_strlen(*redir_op);
	while (*ptr && ft_isspace(*ptr))
		ptr++;
	start = ptr;
	while (*ptr && !ft_isspace(*ptr))
		ptr++;
	*filename = ft_substr(start, 0, ptr - start);
}

static void	handle_redir_only(char **commands, int i, const char *cmd_start)
{
	char	*redir_op;
	char	*filename;

	if (!commands[i + 1])
		return ;
	parse_redir_target(cmd_start, &redir_op, &filename);
	if (redir_op && filename)
	{
		append_redirection(&commands[i + 1], redir_op, filename);
		free(commands[i]);
		commands[i] = ft_strdup("");
	}
	free(redir_op);
	free(filename);
}

void	check_for_redirections(char **commands)
{
	int			i;
	const char	*cmd_start;

	i = 0;
	while (commands[i])
	{
		if (!commands[i][0])
		{
			i++;
			continue ;
		}
		cmd_start = commands[i];
		while (*cmd_start && ft_isspace(*cmd_start))
			cmd_start++;
		if (*cmd_start == '<' && cmd_start[1] != '\0')
		{
			if (is_redirection_only_command(commands[i]))
				handle_redir_only(commands, i, cmd_start);
			else
				move_redirection(commands, i, cmd_start);
		}
		i++;
	}
}
