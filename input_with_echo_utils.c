/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_with_echo_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:17:15 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/16 11:30:20 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_args(char **args)
{
	int	count;

	count = 0;
	while (args && args[count])
		count++;
	return (count);
}

static void	copy_args(char **dst, char **src, int start)
{
	int	i;

	if (!dst || !src)
		return ;
	i = 0;
	while (src[i])
	{
		dst[start + i] = ft_strdup(src[i]);
		if (!dst[start + i])
		{
			// If strdup fails, free previously allocated strings and return
			while (i-- > 0)
				free(dst[start + i]);
			return ;
		}
		//printf("dst[%d]: %s\n", start + i, dst[start + i]);
		i++;
	}
}

static int	count_valid_args(char **args)
{
	int	i;
	int	count;

	if (!args)
		return (0);
	i = -1;
	count = 0;
	while (args[++i])
		if (!is_empty_quoted_string(args[i]))
			count++;
	return (count);
}

char	**duplicate_non_empty_args(char **args, int *count)
{
	char	**result;
	int		i;
	int		j;

	if (!count || !args)
		return (NULL);
	*count = count_valid_args(args);
	result = ft_calloc(*count + 1, sizeof(char *));
	if (!result)
		return (NULL);
	i = -1;
	j = 0;
	while (args[++i] && j < *count)
	{
		if (!is_empty_quoted_string(args[i]))
		{
			result[j] = ft_strdup(args[i]);
			if (!result[j])
				return (free_array(result), NULL);
			j++;
		}
	}
	return (result);
}

int	merge_echo_and_args(char ***args_ptr, char **raw_args, char **echo_args)
{
	char	**final_args;
	int		raw_len;
	int		echo_len;
	char	**args_to_copy;

	if (!args_ptr)
		return (1);
	raw_len = count_args(raw_args);
	echo_len = count_args(echo_args);
	if (echo_len + raw_len == 0)
	{
		*args_ptr = NULL;
		return (0);
	}
	final_args = ft_calloc(echo_len + raw_len + 1, sizeof(char *));
	if (!final_args)
		return (1);
	copy_args(final_args, echo_args, 0);
	args_to_copy = NULL;
	if (raw_args)
		args_to_copy = raw_args + 1;  // Skip the first argument (which is likely "echo")
	//printf("-----------------------------\n");
	/*if (args_to_copy && args_to_copy[0])
		printf("to copy: %s\n", args_to_copy[0]);*/
	if (raw_len > 0)
		copy_args(final_args, args_to_copy, echo_len);

	//free_array(args_to_copy);

	//free(args_to_copy[0]);
	*args_ptr = final_args;
	//free(final_args);

	// print copied args
	/*for (int i = 0; final_args[i]; i++)
	{
		printf("final_args copied[%d]: %s\n", i, final_args[i]);
	}*/




	return (0);
}
