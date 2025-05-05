/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:08:02 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/05 13:44:17 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*trim_spaces(const char *input)
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

void	handle_signal_status(t_shell *shell)
{
	if (g_signal_status)
	{
		shell->exit_status = 130;
		g_signal_status = 0;
	}
}

int	is_empty_quoted_string(const char *str)
{
	return (str[0] == '\"' && str[1] == '\"' && str[2] == '\0');
}
