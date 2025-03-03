/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 11:47:11 by carlos-j          #+#    #+#             */
/*   Updated: 2025/02/28 18:51:07 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*last_exit_status(char *input)
{
	char	*result;
	char	*ptr;
	char	*exit_status_str;
	int		new_len;
	int		exit_len;

	exit_status_str = ft_itoa(g_exit_status);
	if (!exit_status_str)
		return (NULL);
	exit_len = ft_strlen(exit_status_str);
	new_len = ft_strlen(input) + exit_len;
	result = malloc(new_len + 1);
	if (!result)
	{
		free(exit_status_str);
		return (NULL);
	}
	ptr = result;
	while (*input)
	{
		if (*input == '$' && *(input + 1) == '?')
		{
			ft_memcpy(ptr, exit_status_str, exit_len);
			ptr += exit_len;
			input += 2;
		}
		else
			*ptr++ = *input++;
	}
	*ptr = '\0';
	free(exit_status_str);
	return (result);
}
