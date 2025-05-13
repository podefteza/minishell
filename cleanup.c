/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 15:44:13 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/13 11:17:59 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Modified free_array with debug prints
void free_array(char **array)
{
    int i;

    if (!array)
        return;

    //printf("DEBUG: free_array called on %p\n", (void *)array);
    i = 0;
    while (array[i])
    {
        //printf("DEBUG: freeing array[%d]: '%s' at %p\n", i, array[i], (void *)array[i]);
        free(array[i]);
        array[i] = NULL;
        i++;
    }
   // printf("DEBUG: freeing array itself at %p\n", (void *)array);
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
			i++;
		}
		free(shell->envp);
		shell->envp = NULL;
	}
	i = 0;
	if (shell->export_list)
	{
		while (shell->export_list[i] != NULL)
		{
			free(shell->export_list[i]);
			i++;
		}
		free(shell->export_list);
		shell->export_list = NULL;
	}
}
