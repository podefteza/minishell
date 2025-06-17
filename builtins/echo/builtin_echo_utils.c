/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 08:42:56 by carlos-j          #+#    #+#             */
/*   Updated: 2025/06/17 13:43:47 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	skip_n_flags(char **args, int *newline)
{
	int	i;
	int	j;

	i = 1;
	*newline = 1;
	while (args[i] && args[i][0] == '-' && args[i][1] != '\0')
	{
		j = 1;
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] == '\0')
			*newline = 0;
		else
			break ;
		i++;
	}
	return (i);
}

int	is_redirection_token(char *token)
{
	return ((ft_strncmp(token, "<", 1) == 0 && ft_strlen(token) == 1)
		|| (ft_strncmp(token, "<<", 2) == 0 && ft_strlen(token) == 2)
		|| (ft_strncmp(token, ">", 1) == 0 && ft_strlen(token) == 1)
		|| (ft_strncmp(token, ">>", 2) == 0 && ft_strlen(token) == 2));
}

int	is_quoted_redirection(char *token)
{
	int	len;

	if (!token || !is_quoted(token))
		return (0);
	len = ft_strlen(token);
	if (len < 3)
		return (0);
	if ((token[0] == '"' && token[len - 1] == '"')
		|| (token[0] == '\'' && token[len - 1] == '\''))
	{
		if (len == 3 && (token[1] == '<' || token[1] == '>' || token[1] == '|'))
			return (1);
		if (len == 4 && ((token[1] == '<' && token[2] == '<')
				|| (token[1] == '>' && token[2] == '>')))
			return (1);
	}
	return (0);
}
