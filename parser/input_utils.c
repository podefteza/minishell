/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:08:02 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/21 15:10:05 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*trim_spaces(const char *input)
{
	int		start;
	int		end;
	char	*trimmed;
	int		i;

	if (!input)
		return (NULL);
	start = 0;
	while (input[start] == ' ' || input[start] == '\t')
		start++;
	end = ft_strlen(input) - 1;
	while (end > start && (input[end] == ' ' || input[end] == '\t'))
		end--;
	trimmed = malloc((end - start + 2) * sizeof(char));
	if (!trimmed)
		return (NULL);
	i = 0;
	while (start <= end)
		trimmed[i++] = input[start++];
	trimmed[i] = '\0';
	return (trimmed);
}

char	*process_initial_input(char *input)
{
	char	*final_input;
	char	*tmp;

	final_input = trim_spaces(input);
	free(input);
	if (!final_input || count_quotes(final_input))
	{
		if (final_input)
			free(final_input);
		return (NULL);
	}
	tmp = expand_tilde_unquoted(final_input);
	if (!tmp)
	{
		free(final_input);
		return (NULL);
	}
	free(final_input);
	return (tmp);
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

void	remove_quotes_from_commands(t_input *input)
{
	int		i;
	int		j;
	char	*cleaned;

	i = 0;
	while (input->commands && input->commands[i])
	{
		j = 0;
		while (input->commands[i][j])
		{
			cleaned = remove_quotes_concat(input->commands[i][j]);
			free(input->commands[i][j]);
			input->commands[i][j] = cleaned;
			j++;
		}
		i++;
	}
}
