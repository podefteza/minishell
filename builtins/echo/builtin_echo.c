/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:59:20 by carlos-j          #+#    #+#             */
/*   Updated: 2025/04/07 15:09:47 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	builtin_echo(char **args, t_shell *shell)
{
	int i;
	int newline;
	int original_stdout;
	int original_stdin;

	original_stdout = dup(STDOUT_FILENO);
	original_stdin = dup(STDIN_FILENO);
	if (original_stdout == -1 || original_stdin == -1)
	{
		perror("dup");
		return (1);
	}
	if (handle_redirections(args, shell) == -1)
	{
		dup2(original_stdout, STDOUT_FILENO);
		dup2(original_stdin, STDIN_FILENO);
		close(original_stdout);
		close(original_stdin);
		return (shell->exit_status);
	}
	i = 1;
	newline = 1;
	if (args[i] && ft_strncmp(args[i], "-n", 3) == 0)
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		if ((ft_strncmp(args[i], "<", ft_strlen(args[i])) == 0)
			|| (ft_strncmp(args[i], "<<", ft_strlen(args[i])) == 0)
			|| (ft_strncmp(args[i], ">", ft_strlen(args[i])) == 0)
			|| (ft_strncmp(args[i], ">>", ft_strlen(args[i])) == 0))
		{
			if (args[i + 1])
				i += 2;
			else
				i += 1;
			continue ;
		}
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	dup2(original_stdout, STDOUT_FILENO);
	dup2(original_stdin, STDIN_FILENO);
	close(original_stdout);
	close(original_stdin);
	return (0);
}
