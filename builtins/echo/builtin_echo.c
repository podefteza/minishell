/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:59:20 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/28 14:11:39 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	skip_n_flags(char **args, int *newline)
{
	int	i;
	int	j;

	i = 1;
	*newline = 1;
	while (args[i] && args[i][0] == '-' && args[i][1] != '\0')
	{
		j = 1;
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] == '\0')
			*newline = 0;
		else
			break ;
		i++;
	}
	return (i);
}

static void	print_echo_arguments(char **args, int i)
{
	char	*tmp;

	while (args[i])
	{
		if (!is_quoted(args[i]) && is_redirection_token(args[i]))
		{
			if (args[i + 1])
				i += 2;
			else
				i++;
			continue ;
		}
		tmp = handle_quotes(args[i]);
		if (printf("%s", tmp) < 0)
			break ;
		if (args[i + 1])
			printf(" ");
		i++;
		free(tmp);
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
