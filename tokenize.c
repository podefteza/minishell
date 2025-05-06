/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:29:25 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/06 17:15:34 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_tokens(char *input)
{
	int		total_tokens;
	char	*temp_ptr;
	char	*token;
	char	*temp_input;

	total_tokens = 0;
	temp_input = ft_strdup(input);
	if (!temp_input)
		return (-1);
	temp_ptr = temp_input;
	while (1)
	{
		token = get_next_token(&temp_ptr);
		if (!token)
			break ;
		total_tokens++;
		free(token);
	}
	free(temp_input);
	return (total_tokens);
}

static int	fill_args_array(char **args, char *input, int total_tokens)
{
	int		i;
	char	*input_ptr;
	char	*token;

	input_ptr = input;
	i = 0;
	while (i < total_tokens)
	{
		token = get_next_token(&input_ptr);
		if (!token)
			break ;
		args[i] = ft_strdup(token);
		free(token);
		if (!args[i])
		{
			while (i-- > 0)
				free(args[i]);
			return (0);
		}
		i++;
	}
	return (1);
}

char	**split_arguments(char *input)
{
	char	**args;
	int		total_tokens;

	if (!input)
		return (NULL);
	total_tokens = count_tokens(input);
	if (total_tokens < 0)
		return (NULL);
	args = malloc(sizeof(char *) * (total_tokens + 1));
	args[total_tokens] = NULL;
	if (!args)
		return (NULL);
	if (!fill_args_array(args, input, total_tokens))
	{
		free(args);
		return (NULL);
	}
	return (args);
}
