/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:18:48 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/23 14:01:43 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	is_tilde_expansion_point(char *input, int i)
{
	if (input[i] != '~')
		return (0);
	if (i > 0 && input[i - 1] != ' ')
		return (0);
	if (input[i + 1] && input[i + 1] != '/' && input[i + 1] != ' ')
		return (0);
	return (1);
}

static void	update_quote_state(char c, int *in_single, int *in_double)
{
	if (c == '\'' && !(*in_double))
		*in_single = !(*in_single);
	else if (c == '"' && !(*in_single))
		*in_double = !(*in_double);
}

static int	copy_home(char *result, int j, char *home)
{
	int	k;

	k = 0;
	while (home[k])
	{
		result[j] = home[k];
		j++;
		k++;
	}
	return (j);
}

static void	expand_loop(char *input, char *result, char *home)
{
	int	i;
	int	j;
	int	in_single;
	int	in_double;

	i = 0;
	j = 0;
	in_single = 0;
	in_double = 0;
	while (input[i])
	{
		update_quote_state(input[i], &in_single, &in_double);
		if (!in_single && !in_double && is_tilde_expansion_point(input, i))
		{
			j = copy_home(result, j, home);
			i++;
		}
		else
		{
			result[j] = input[i];
			j++;
			i++;
		}
	}
	result[j] = '\0';
}

char	*expand_tilde_unquoted(char *input)
{
	char	*home;
	char	*result;

	home = getenv("HOME");
	if (!home)
		return (ft_strdup(input));
	result = malloc(ft_strlen(input) + ft_strlen(home) + 1);
	if (!result)
		return (NULL);
	expand_loop(input, result, home);
	return (result);
}
