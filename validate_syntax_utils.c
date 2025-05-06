/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntax_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:03:50 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/06 16:04:11 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_syntax_error(char *token)
{
	char	error_token[3];

	error_token[0] = 0;
	error_token[1] = 0;
	error_token[2] = 0;
	ft_putstr_fd("minishell: syntax error near unexpected token `",
		STDERR_FILENO);
	if (token && *token)
	{
		if (*token == '<' || *token == '>')
		{
			error_token[0] = *token;
			if (*(token + 1) == *token)
				error_token[1] = *token;
			ft_putstr_fd(error_token, STDERR_FILENO);
		}
		else if (*token == '|')
			ft_putstr_fd("|", STDERR_FILENO);
		else
			ft_putstr_fd(token, STDERR_FILENO);
	}
	else
		ft_putstr_fd("newline", STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
}
