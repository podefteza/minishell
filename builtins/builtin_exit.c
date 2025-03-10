/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:57:11 by carlos-j          #+#    #+#             */
/*   Updated: 2025/03/10 13:16:19 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	validate_and_get_exit_code(char *exit_code_str, char **args, int i)
{
	long	exit_code;

	while (exit_code_str[i])
	{
		if (!ft_isdigit(exit_code_str[i]))
		{
			ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
			free(exit_code_str);
			free_args(args);
			exit(2);
		}
		i++;
	}
	exit_code = ft_atoi(exit_code_str);
	free(exit_code_str);
	return ((unsigned char)exit_code);
}

int	handle_exit_in_first_arg(char **args)
{
	int		i;
	long	exit_code;
	char	*exit_code_str;
	int		k;

	exit_code_str = NULL;
	k = 4;
	while (args[0][k] && args[0][k] == ' ')
		k++;
	if (!args[0][k])
		return (0);
	exit_code_str = ft_strdup(&args[0][k]);
	if (!exit_code_str)
		return (0);
	i = 0;
	if (exit_code_str[i] == '+' || exit_code_str[i] == '-')
		i++;
	exit_code = validate_and_get_exit_code(exit_code_str, args, i);
	return (exit_code);
}

int	handle_exit_in_second_arg(char **args)
{
	int		i;
	long	exit_code;

	i = 0;
	if (args[1][i] == '+' || args[1][i] == '-')
		i++;
	while (args[1][i])
	{
		if (!ft_isdigit(args[1][i]))
		{
			ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
			free_args(args);
			exit(2);
		}
		i++;
	}
	if (args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (-1);
	}
	exit_code = ft_atoi(args[1]);
	return ((unsigned char)exit_code);
}

int	builtin_exit_with_code(char **args)
{
	if (!args[1] && ft_strlen(args[0]) > 4)
		return (handle_exit_in_first_arg(args));
	if (args[1])
		return (handle_exit_in_second_arg(args));
	return (0);
}

void	builtin_exit(char **args, int *exit_status)
{
	int	exit_code;

	if (!args || !args[0])
		exit_code = 0;
	else
	{
		exit_code = builtin_exit_with_code(args);
		if (exit_code == -1)
		{
			*exit_status = 1;
			return ;
		}
	}
	ft_putstr_fd("exit\n", 1);
	free_args(args);
	exit(exit_code);
}
