/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 09:11:45 by carlos-j          #+#    #+#             */
/*   Updated: 2025/04/21 14:40:10 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_puterr(char *msg1, char *msg2, char *msg3, char *msg4)
{
	char	buffer[1024];
	int		offset;

	offset = 0;
	if (msg1)
		offset += ft_strlcpy(buffer + offset, msg1, sizeof(buffer) - offset);
	if (msg2)
		offset += ft_strlcpy(buffer + offset, msg2, sizeof(buffer) - offset);
	if (msg3)
		offset += ft_strlcpy(buffer + offset, msg3, sizeof(buffer) - offset);
	if (msg4)
		offset += ft_strlcpy(buffer + offset, msg4, sizeof(buffer) - offset);
	write(STDERR_FILENO, buffer, offset);
}
