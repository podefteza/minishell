/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:08:02 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/23 14:43:07 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	trim_spaces(t_shell *shell)
{
	int		start;
	int		end;
	int		i;

	if (!shell->input.raw)
		return ;
	start = 0;
	while (shell->input.raw[start] == ' ' || shell->input.raw[start] == '\t')
		start++;
	end = ft_strlen(shell->input.raw) - 1;
	while (end > start && (shell->input.raw[end] == ' ' || shell->input.raw[end] == '\t'))
		end--;
	shell->input.trim_spaces = malloc((end - start + 2) * sizeof(char));
	if (!shell->input.trim_spaces)
		return ;
	i = 0;
	while (start <= end)
		shell->input.trim_spaces[i++] = shell->input.raw[start++];
	shell->input.trim_spaces[i] = '\0';
}

void	process_initial_input(t_shell *shell)
{
	trim_spaces(shell);
	free(shell->input.raw);
	shell->input.raw = NULL;
	if (!shell->input.trim_spaces || count_quotes(shell->input.trim_spaces))
	{
		free(shell->input.trim_spaces);
		shell->input.trim_spaces = NULL;
		return ;
	}
	shell->input.processed = expand_tilde_unquoted(shell->input.trim_spaces);
	free(shell->input.trim_spaces);
	shell->input.trim_spaces = NULL;
	if (!shell->input.processed)
		return ;
}


void	handle_signal_status(t_shell *shell)
{
	if (g_signal_status)
	{
		shell->exit_status = 130;
		g_signal_status = 0;
	}
}

static char	*remove_quotes_concat(const char *str)
{
	int		i;
	int		j;
	char	quote;
	char	*result;

	i = 0;
	j = 0;
	quote = 0;
	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	while (str[i])
	{
		if ((str[i] == '"' || str[i] == '\'') && !quote)
			quote = str[i++];
		else if (str[i] == quote)
		{
			quote = 0;
			i++;
		}
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

int remove_quotes_from_commands(t_shell *shell)
{
    int     i;
    int     j;
    char    *cleaned;

    i = 0;
    while (shell->input.commands && shell->input.commands[i])
    {
        j = 0;
        while (shell->input.commands[i][j])
        {
            cleaned = remove_quotes_concat(shell->input.commands[i][j]);
            if (!cleaned)
            {
                // Free all previously allocated memory
                while (i >= 0)
                {
                    while (shell->input.commands[i] && j >= 0)
                    {
                        free(shell->input.commands[i][j]);
                        shell->input.commands[i][j] = NULL;
                        j--;
                    }
                    i--;
                }
                return (1); // Memory allocation error
            }

            // Check if the cleaned string is empty
            if (cleaned[0] == '\0')
            {
                free(cleaned);
                // Free all previously allocated memory
                while (i >= 0)
                {
                    while (shell->input.commands[i] && j >= 0)
                    {
                        free(shell->input.commands[i][j]);
                        shell->input.commands[i][j] = NULL;
                        j--;
                    }
                    i--;
                }
                return (2); // Empty command error (you can use any error code you prefer)
            }

            free(shell->input.commands[i][j]);
            shell->input.commands[i][j] = cleaned;
            j++;
        }
        i++;
    }
    return (0); // Success
}
