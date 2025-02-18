/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 14:09:00 by carlos-j          #+#    #+#             */
/*   Updated: 2025/02/18 14:09:27 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_cd(char **args)
{
	if (!args[1])
	{
		char *home = getenv("HOME");
		if (home)
		{
			if (chdir(home) != 0)
				perror("minishell: cd");
		}
		else
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
	}
	else
	{
		if (chdir(args[1]) != 0)
			perror("minishell: cd");
	}
}

void	builtin_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		perror("minishell: pwd");
}

void	builtin_echo(char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	printf("\n");
}
