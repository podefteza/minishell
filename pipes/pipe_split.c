/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:58:40 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/23 09:07:23 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	free_split_result(char **result, int *index)
{
	while (*index > 0)
		free(result[--(*index)]);
	free(result);
}

static int	add_segment(char *start, char *end, char **result, int *index)
{
	char	*temp;
	char	*trimmed;

	temp = malloc(end - start + 1);
	if (!temp)
		return (0);
	ft_strlcpy(temp, start, end - start + 1);
	trimmed = ft_strtrim(temp, " ");
	free(temp);
	if (!trimmed)
		return (0);
	result[(*index)++] = trimmed;
	result[(*index)] = NULL;
	return (1);
}

static int	update_and_split(char **result, int *arg_index, char *start,
		char *end)
{
	if (!add_segment(start, end, result, arg_index))
		return (0);
	result[(*arg_index)++] = ft_strdup("|");
	if (!result[*arg_index - 1])
		return (0);
	return (1);
}

static int	split_loop(char *input, char **result, int *arg_index)
{
	char	*start;
	int		in_quotes;
	char	quote_char;
	int		i;

	start = input;
	in_quotes = 0;
	i = 0;
	while (input[i])
	{
		check_quote_state(input[i], &in_quotes, &quote_char);
		if (!in_quotes && input[i] == '|')
		{
			if (!update_and_split(result, arg_index, start, &input[i]))
				return (free_split_result(result, arg_index), 0);
			start = &input[i + 1];
		}
		i++;
	}
	if (!add_segment(start, &input[i], result, arg_index))
		return (free_split_result(result, arg_index), 0);
	result[*arg_index] = NULL;
	return (1);
}

char	**split_pipe(t_shell *shell)
{
	char	**result;
	int		arg_index;
	int		num_pipes;

	if (has_trailing_pipe(shell->input.expanded)
		|| has_invalid_pipe_syntax(shell->input.expanded)
		|| shell->input.expanded[0] == '|')
	{
		ft_puterr("minishell", SNT, " `|'", "\n");
		shell->exit_status = 2;
		return (NULL);
	}
	num_pipes = count_pipes(shell->input.expanded);
	result = malloc((num_pipes * 2 + 2) * sizeof(char *));
	if (!result)
		return (NULL);
	arg_index = 0;
	if (!split_loop(shell->input.expanded, result, &arg_index))
		return (NULL);
	return (result);
}
