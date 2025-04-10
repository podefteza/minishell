/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 09:11:45 by carlos-j          #+#    #+#             */
/*   Updated: 2025/04/10 09:12:18 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_puterr(char *msg1, char *msg2, char *msg3, char *msg4)
{
	ft_putstr_fd(msg1, 2);
	ft_putstr_fd(msg2, 2);
	ft_putstr_fd(msg3, 2);
	ft_putstr_fd(msg4, 2);
}
