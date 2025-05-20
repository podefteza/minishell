/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 10:54:06 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/20 11:40:13 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	validate_and_get_exit_code(char *exit_code_str, char **args, int i,
		t_shell *shell)
{
	long	exit_code;

	printf("validate_and_get_exit_code called\n");

	while (exit_code_str[i])
	{
		if (!ft_isdigit(exit_code_str[i]))
		{
			ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
			free(exit_code_str);
			free_array(args);
			free_shell_resources(shell);
			exit(2);
		}
		i++;
	}
	exit_code = ft_atoi(exit_code_str);
	free(exit_code_str);
	return ((unsigned char)exit_code);
}

int	handle_exit_in_first_arg(char **args, t_shell *shell)
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
	exit_code = validate_and_get_exit_code(exit_code_str, args, i, shell);
	return (exit_code);
}

static int	is_str_digit(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	is_overflow(const char *str)
{
	int				sign;
	int				i;
	unsigned long	value;
	unsigned long	cutoff;

	sign = 1;
	value = 0;
	cutoff = (unsigned long)LONG_MAX;
	i = 0;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i++])
	{
		if (value > cutoff / 10)
			return (1);
		if (value == cutoff / 10 && (unsigned long)(str[i - 1] - '0') > (cutoff
				% 10 + (sign < 0)))
			return (1);
		value = value * 10 + (str[i - 1] - '0');
	}
	return (0);
}

int	handle_exit_in_second_arg(char **args, t_shell *shell)
{
	long	exit_code;

	if (!is_str_digit(args[1]) || is_overflow(args[1]))
	{
		ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
		free_array(args);
		free_shell_resources(shell);
		exit(2);
	}
	if (args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (-1);
	}
	exit_code = ft_atoll(args[1]);
	return ((unsigned char)exit_code);
}
