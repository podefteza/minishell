/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:04:57 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/13 10:41:15 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*process_input_for_execution(t_shell *shell)
{
	if (validate_syntax(shell))
	{
		free(shell->input.processed);
		return (NULL);
	}
	shell->input.processed = check_for_expansion(shell->input.processed, shell);
	if (!shell->input.processed || shell->input.processed[0] == '\0')
	{
		free(shell->input.processed);
		return (NULL);
	}
	if (input_with_pipe(shell->input.processed, shell))
	{
		//printf("reach here, has pipe\n");
		//free(input);
		return (NULL);
	}
	//printf("input: [%s]\n", input);
	return (shell->input.processed);
}

static void handle_export_case(t_input *input)
{
    input->args = split_arguments(input->processed);
    if (!input->args || !input->args[0])
    {
        free_array(input->args);
        input->args = NULL;
    }
}

static int handle_echo_case(t_shell *shell)
{
	if (!input_with_echo(shell) || is_quoted(shell->input.processed))
    {
        if (shell->input.args)
        {
            free_array(shell->input.args);
           	shell->input.args = NULL;
        }
        return (0);
    }
    return (1);
}

void parse_command_arguments(t_shell *shell)
{
    if (ft_strnstr(shell->input.processed, "echo", ft_strlen(shell->input.processed)))
    {
        if (!handle_echo_case(shell))
            return;
    }
    else if (ft_strnstr(shell->input.processed, "export", ft_strlen(shell->input.processed)))
    {
        handle_export_case(&shell->input);
    }
    else
    {
		shell->input.args = split_arguments(shell->input.processed);
        if (!shell->input.args || !shell->input.args[0])
        {
            free_array(shell->input.args);
            shell->input.args = NULL;
        }
    }
}

void clean_arguments(t_shell *shell)
{
    int     i;
    char    *cleaned_arg;

    if (!shell || !shell->input.args)
        return;

    i = 0;
    while (shell->input.args[i])
    {
        cleaned_arg = handle_quotes(shell->input.args[i]);
        free(shell->input.args[i]);          // Free the original string
        shell->input.args[i] = cleaned_arg;  // Replace with cleaned string
        i++;
    }
}


