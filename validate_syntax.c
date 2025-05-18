/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 09:39:51 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/16 08:55:49 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	validate_redirection(int *i, t_shell *shell)
{
	char	c;

	c = shell->input.processed[*i];
	(*i)++;
	if (shell->input.processed[*i] == c)
		(*i)++;
	while (shell->input.processed[*i] && ft_isspace(shell->input.processed[*i]))
		(*i)++;
	if (shell->input.processed[*i] == '\0' || shell->input.processed[*i] == '|' || shell->input.processed[*i] == '<'
		|| shell->input.processed[*i] == '>')
	{
		if (shell->input.processed[*i] == '\0')
			print_syntax_error("newline");
		else
			print_syntax_error(&shell->input.processed[*i]);
		shell->exit_status = 2;
		return (1);
	}
	return (0);
}

static int	validate_pipe(int *i, t_shell *shell)
{
	(*i)++;
	while (shell->input.processed[*i] && ft_isspace(shell->input.processed[*i]))
		(*i)++;
	if (shell->input.processed[*i] == '\0' || shell->input.processed[*i] == '|')
	{
		if (shell->input.processed[*i] == '\0')
			print_syntax_error("newline");
		else
			print_syntax_error(&shell->input.processed[*i]);
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

static int	search_syntax_errors(int *i, t_shell *shell)
{
	while (shell->input.processed[*i])
	{
		if (shell->input.processed[*i] == '\'' || shell->input.processed[*i] == '\"')
		{
			if (skip_inside_quotes(shell->input.processed, i))
				return (1);
		}
		else if (shell->input.processed[*i] == '<' || shell->input.processed[*i] == '>')
		{
			if (validate_redirection(i, shell))
				return (1);
		}
		else if (shell->input.processed[*i] == '|')
		{
			if (validate_pipe(i, shell))
				return (1);
		}
		else
			(*i)++;
	}
	return (0);
}

int	validate_syntax(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->input.processed[i] && ft_isspace(shell->input.processed[i]))
		i++;
	if (shell->input.processed[i] == '|')
	{
		print_syntax_error(&shell->input.processed[i]);
		return (shell->exit_status = 2);
	}
	if (search_syntax_errors(&i, shell))
		return (1);
	return (0);
}
