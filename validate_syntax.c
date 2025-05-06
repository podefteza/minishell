/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 09:39:51 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/06 16:03:58 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	validate_redirection(char *input, int *i, t_shell *shell)
{
	char	c;

	c = input[*i];
	(*i)++;
	if (input[*i] == c)
		(*i)++;
	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
	if (input[*i] == '\0' || input[*i] == '|' || input[*i] == '<'
		|| input[*i] == '>')
	{
		if (input[*i] == '\0')
			print_syntax_error("newline");
		else
			print_syntax_error(&input[*i]);
		shell->exit_status = 2;
		return (1);
	}
	return (0);
}

static int	validate_pipe(char *input, int *i, t_shell *shell)
{
	(*i)++;
	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
	if (input[*i] == '\0' || input[*i] == '|')
	{
		if (input[*i] == '\0')
			print_syntax_error("newline");
		else
			print_syntax_error(&input[*i]);
		shell->exit_status = 2;
		return (1);
	}
	return (0);
}

static int	skip_inside_quotes(char *input, int *i)
{
	char	c;

	c = input[*i];
	(*i)++;
	while (input[*i] && input[*i] != c)
		(*i)++;
	if (input[*i])
		(*i)++;
	return (0);
}

static int	search_syntax_errors(char *input, int *i, t_shell *shell)
{
	while (input[*i])
	{
		if (input[*i] == '\'' || input[*i] == '\"')
		{
			if (skip_inside_quotes(input, i))
				return (1);
		}
		else if (input[*i] == '<' || input[*i] == '>')
		{
			if (validate_redirection(input, i, shell))
				return (1);
		}
		else if (input[*i] == '|')
		{
			if (validate_pipe(input, i, shell))
				return (1);
		}
		else
			(*i)++;
	}
	return (0);
}

int	validate_syntax(char *input, t_shell *shell)
{
	int	i;

	i = 0;
	while (input[i] && ft_isspace(input[i]))
		i++;
	if (input[i] == '|')
	{
		print_syntax_error(&input[i]);
		return (shell->exit_status = 2);
	}
	if (search_syntax_errors(input, &i, shell))
		return (1);
	return (0);
}
