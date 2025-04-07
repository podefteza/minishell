/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 15:44:13 by carlos-j          #+#    #+#             */
/*   Updated: 2025/04/07 12:30:54 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	free_shell_resources(t_shell *shell)
{
	int	i;

	i = 0;
	if (shell->envp)
	{
		while (shell->envp[i] != NULL)
		{
			free(shell->envp[i]);
			shell->envp[i] = NULL;
			i++;
		}
		free(shell->envp);
		shell->envp = NULL;
	}
}
