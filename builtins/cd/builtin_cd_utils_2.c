/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_utils_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 09:15:37 by carlos-j          #+#    #+#             */
/*   Updated: 2025/06/16 09:17:09 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_tilde_expansion_point(char *input, int i)
{
	if (input[i] != '~')
		return (0);
	if (i > 0 && input[i - 1] != ' ')
		return (0);
	return (1);
}

int	get_username_length(char *input, int start)
{
	int	len;
	int	i;

	len = 0;
	i = start;
	while (input[i] && input[i] != '/' && input[i] != ' ')
	{
		len++;
		i++;
	}
	return (len);
}

int	is_current_user(char *input, int start, int len, t_shell *shell)
{
	if (len == 0)
		return (1);
	if (len != (int)ft_strlen(shell->user))
		return (0);
	return (ft_strncmp(input + start, shell->user, len) == 0);
}

void	update_quote_state(char c, int *in_single, int *in_double)
{
	if (c == '\'' && !(*in_double))
		*in_single = !(*in_single);
	else if (c == '"' && !(*in_single))
		*in_double = !(*in_double);
}

int	copy_home(char *result, int j, char *home)
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
