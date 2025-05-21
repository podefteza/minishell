/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 15:44:13 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/21 20:50:20 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		if (array[i])
		{
			free(array[i]);
			array[i] = NULL;
		}
		i++;
	}
	free(array);
}

/*void free_commands_array(char ***commands)
{
	int	i;
	int	i;

	if (!commands)
		return ;
	i = 0;
	while (commands[i])
	{
		free_array(commands[i]);
		i++;
	}
	free(commands);
}*/
void	free_input(t_input *input)
{
	int i;

	i = 0;
	if (!input)
		return ;
	if (input->raw)
	{
		free(input->raw);
		input->raw = NULL;
	}
	/*if (input->processed)
	{
		free(input->processed);
		input->processed = NULL;
	}*/
	if (input->args)
	{
		free_array(input->args);
		input->args = NULL;
	}
	if (input->commands)
	{
		i = 0;
		while (input->commands[i])
		{
			free_array(input->commands[i]);
			i++;
		}
		free(input->commands);
		input->commands = NULL;
	}
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
