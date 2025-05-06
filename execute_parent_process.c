/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_parent_process.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 14:16:52 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/06 15:05:16 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_parent_process(int *input_fd, int pipe_fds[2])
{
	if (pipe_fds[1] != -1)
		close(pipe_fds[1]);
	if (*input_fd != STDIN_FILENO)
		close(*input_fd);
	if (pipe_fds[0] != -1)
		*input_fd = pipe_fds[0];
}
