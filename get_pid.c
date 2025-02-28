/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_pid.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:34:27 by carlos-j          #+#    #+#             */
/*   Updated: 2025/02/28 14:41:57 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t	last_background_pid(pid_t pid)
{
	static pid_t	last_pid;

	if (pid != 0)
		last_pid = pid;
	return (last_pid);
}
