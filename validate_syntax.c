/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 09:39:51 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/05 15:07:45 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	skip_spaces(char *input, int i)
{
	while (input[i] && (input[i] == ' ' || input[i] == '\t'))
		i++;
	return (i);
}

static void	syntax_error(char *token)
{
	char	error_token[3] = {0};

	ft_putstr_fd("minishell: syntax error near unexpected token `",
		STDERR_FILENO);
	if (token && *token)
	{
		if (*token == '<' || *token == '>')
		{
			error_token[0] = *token;
			if (*(token + 1) == *token)
				error_token[1] = *token;
			ft_putstr_fd(error_token, STDERR_FILENO);
		}
		else if (*token == '|')
			ft_putstr_fd("|", STDERR_FILENO);
		else
			ft_putstr_fd(token, STDERR_FILENO);
	}
	else
		ft_putstr_fd("newline", STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
}

int	validate_syntax(char *input, t_shell *shell)
{
	int		i;
	char	c;

	i = skip_spaces(input, 0);
	if (input[i] == '|')
	{
		syntax_error(&input[i]);
		shell->exit_status = 2;
		return (1);
	}
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '\"')
		{
			c = input[i++];
			while (input[i] && input[i] != c)
				i++;
			if (input[i])
				i++;
			continue ;
		}
		else if (input[i] == '<' || input[i] == '>')
		{
			c = input[i];
			i++;
			if (input[i] == c)
				i++;
			i = skip_spaces(input, i);
			if (input[i] == '\0' || input[i] == '|' || input[i] == '<'
				|| input[i] == '>')
			{
				syntax_error(&input[i] ? &input[i] : "newline");  // cant use ternaries!!
				shell->exit_status = 2;
				return (1);
			}
		}
		else if (input[i] == '|')
		{
			i++;
			i = skip_spaces(input, i);
			if (input[i] == '\0' || input[i] == '|')
			{
				syntax_error(input[i] ? &input[i] : "newline"); // cant use ternaries!!
				shell->exit_status = 2;
				return (1);
			}
		}
		else
			i++;
	}
	return (0);
}
