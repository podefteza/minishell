/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 13:55:32 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/08 13:08:13 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	input_with_pipe(char *final_input, t_shell *shell)
{
	char	**commands;

	if (!ft_strchr(final_input, '|') || !is_pipe_outside_quotes(final_input))
		return (0);
	commands = split_pipe(final_input, shell);
	if (!commands)
		return (1);
	execute_pipeline(commands, shell);
	free_array(commands);
	return (1);
}

int	validate_executable(char **args, t_shell *shell)
{
	if (args[0] && args[0][0] == '/')
	{
		if (access(args[0], F_OK) != 0)
		{
			ft_puterr("minishell: ", args[0], ": No such file or directory\n",
				"");
			free_array(args);
			shell->exit_status = 127;
			return (0);
		}
	}
	return (1);
}

void	execute_final_command(char **args, t_shell *shell)
{
	if (execute_builtin(args, shell))
	{
		if (args)
			free_array(args);
		return ;
	}
	execute_command(args, shell);
	if (args)
		free_array(args);
}



void handle_input(char *input, t_shell *shell)
{
    char    **args;
    char    *final_input;

    handle_signal_status(shell);
    final_input = process_initial_input(input);
    if (!final_input)
        return;
   // printf("final_input: %s\n", final_input);
    final_input = process_input_for_execution(final_input, shell);
    if (!final_input)
        return;
    //printf("final_input: %s\n", final_input);
    args = parse_command_arguments(final_input, shell);
    if (!args)
        return;

    // Print args before cleaning if needed
    /*for (int i = 0; args[i]; i++)
    {
        printf("args[%d]: %s\n", i, args[i]);
    }*/

    // Use the updated clean_arguments that handles both passes
    clean_arguments(args);

    // Print args after cleaning if needed
    /*printf("After cleaning:\n");
    for (int i = 0; args[i]; i++)
    {
        printf("args[%d]: %s\n", i, args[i]);
    }*/

    if (!validate_executable(args, shell))
        return;
    execute_final_command(args, shell);
}
