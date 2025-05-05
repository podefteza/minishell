/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:56:32 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/05 14:58:59 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redirection_operator(char *str)
{
	if (!str)
		return (0);
	if (ft_strncmp(str, "<", 2) == 0 || ft_strncmp(str, ">", 2) == 0
		|| ft_strncmp(str, "<<", 3) == 0 || ft_strncmp(str, ">>", 3) == 0)
		return (1);
	return (0);
}

int	is_invalid_redirection(char *token)
{
	if (!token)
		return (0);
	if ((token[0] == '>' || token[0] == '<') && (token[1] == '>'
			|| token[1] == '<' || token[1] == '\0'))
	{
		if ((ft_strncmp(token, ">", 2) == 0 && token[1] == '\0')
			|| (ft_strncmp(token, ">>", 3) == 0 && token[2] == '\0')
			|| (ft_strncmp(token, "<", 2) == 0 && token[1] == '\0')
			|| (ft_strncmp(token, "<<", 3) == 0 && token[2] == '\0'))
			return (0);
		return (1);
	}
	return (0);
}

char	*get_redirection_symbol(char *token)
{
	if (!token)
		return (NULL);
	if (ft_strncmp(token, ">>", 3) == 0 && token[2] == '\0')
		return (">>");
	if (ft_strncmp(token, "<<", 3) == 0 && token[2] == '\0')
		return ("<<");
	if (ft_strncmp(token, ">", 2) == 0 && token[1] == '\0')
		return (">");
	if (ft_strncmp(token, "<", 2) == 0 && token[1] == '\0')
		return ("<");
	if (token[0] == '>')
	{
		if (token[1] == '>')
			return (">>");
		return (">");
	}
	if (token[0] == '<')
	{
		if (token[1] == '<')
			return ("<<");
		return ("<");
	}
	return (token);
}

int	open_redirection_file(char *op, char *filename)
{
	if (ft_strncmp(op, ">", 2) == 0)
		return (open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	if (ft_strncmp(op, ">>", 3) == 0)
		return (open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644));
	if (ft_strncmp(op, "<", 2) == 0)
		return (open(filename, O_RDONLY));
	if (ft_strncmp(op, "<<", 3) == 0)
		return (handle_heredoc(filename));
	return (-1);
}

void	print_redirection_syntax_error(t_shell *shell, char *token)
{
	ft_puterr("minishell", SNT, " `", get_redirection_symbol(token));
	ft_putstr_fd("'\n", STDERR_FILENO);
	shell->exit_status = 2;
}
