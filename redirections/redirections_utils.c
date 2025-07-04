/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:56:32 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/28 14:08:06 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_redirection_operator(char *str)
{
	if (!str)
		return (FALSE);
	if (ft_strncmp(str, "<", 2) == 0 || ft_strncmp(str, ">", 2) == 0
		|| ft_strncmp(str, "<<", 3) == 0 || ft_strncmp(str, ">>", 3) == 0)
		return (TRUE);
	return (FALSE);
}

int	open_redirection_file(char *op, char *filename)
{
	if (ft_strncmp(op, ">", 2) == 0)
		return (open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	if (ft_strncmp(op, ">>", 3) == 0)
		return (open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644));
	if (ft_strncmp(op, "<", 2) == 0)
		return (open(filename, O_RDONLY));
	return (-1);
}
