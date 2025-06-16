/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:59:20 by carlos-j          #+#    #+#             */
/*   Updated: 2025/06/16 08:45:05 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	handle_echo_token(char *token)
{
	int		len;
	char	*content;

	if (!is_quoted(token) && is_redirection_token(token))
		return (0);
	if (is_quoted_redirection(token))
	{
		len = ft_strlen(token);
		content = ft_substr(token, 1, len - 2);
		if (content)
		{
			printf("%s", content);
			free(content);
		}
	}
	else
	{
		if (printf("%s", token) < 0)
			return (-1);
	}
	return (1);
}

static void	print_echo_arguments(char **args, int i)
{
	int	result;

	while (args[i])
	{
		result = handle_echo_token(args[i]);
		if (result == 0)
		{
			if (args[i + 1])
				i += 2;
			else
				i++;
			continue ;
		}
		if (result == -1)
			break ;
		if (args[i + 1])
			printf(" ");
		i++;
	}
}

static void	print_echo_core(char **args, int start, int newline)
{
	if (!args[start])
	{
		if (newline)
			printf("\n");
	}
	else
	{
		print_echo_arguments(args, start);
		if (newline)
			printf("\n");
	}
}

int	builtin_echo(char **args, t_shell *shell)
{
	int	i;
	int	newline;
	int	original_stdout;
	int	original_stdin;

	original_stdout = dup(STDOUT_FILENO);
	original_stdin = dup(STDIN_FILENO);
	if (original_stdout == -1 || original_stdin == -1)
	{
		perror("dup");
		return (-1);
	}
	if (handle_redirections(args, shell) == -1)
	{
		restore_stdio(original_stdout, original_stdin);
		return (shell->exit_status);
	}
	i = skip_n_flags(args, &newline);
	print_echo_core(args, i, newline);
	restore_stdio(original_stdout, original_stdin);
	shell->exit_status = 0;
	return (0);
}
