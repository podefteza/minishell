/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:57:11 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/28 14:14:36 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	builtin_exit_with_code(char **args, t_shell *shell)
{
	if (!args[1] && ft_strlen(args[0]) > 4)
		return (handle_exit_in_first_arg(args, shell));
	if (args[1])
		return (handle_exit_in_second_arg(args, shell));
	return (0);
}

static char	*strip_inner_quotes(const char *str)
{
	char	*result;
	int		i;
	int		j;

	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] != '\'' && str[i] != '"')
		{
			result[j] = str[i];
			j++;
		}
		i++;
	}
	result[j] = '\0';
	return (result);
}

static void	clean_exit_arg(char **args)
{
	char	*cleaned;

	if (args[1])
	{
		cleaned = strip_inner_quotes(args[1]);
		if (cleaned)
		{
			free(args[1]);
			args[1] = cleaned;
		}
	}
}

static void	exit_and_cleanup(t_shell *shell, int exit_code)
{
	ft_putstr_fd("exit\n", 1);
	shell->should_exit = 1;
	free(shell->input.expanded);
	free_array(shell->input.args);
	free_commands_array(shell->input.commands);
	free_shell_resources(shell);
	close_all_fds();
	exit(exit_code);
}

int	builtin_exit(char **args, t_shell *shell)
{
	int	exit_code;

	if (!args || !args[0])
		exit_code = 0;
	else
	{
		clean_exit_arg(args);
		exit_code = builtin_exit_with_code(args, shell);
		if (exit_code == -1)
		{
			shell->should_exit = 0;
			shell->exit_status = 1;
			return (1);
		}
	}
	exit_and_cleanup(shell, exit_code);
	return (0);
}
