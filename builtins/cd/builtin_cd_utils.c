/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:18:48 by carlos-j          #+#    #+#             */
/*   Updated: 2025/04/04 16:23:02 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	cd_error(t_shell *shell)
{
	shell->exit_status = 1;
	ft_putstr_fd("minishell: cd: too many arguments\n", 2);
}

char	*expand_tilde(char *path)
{
	char	*home;
	char	*expanded_path;

	home = getenv("HOME");
	if (home)
	{
		expanded_path = malloc(ft_strlen(home) + ft_strlen(path));
		if (expanded_path)
		{
			ft_strlcpy(expanded_path, home, ft_strlen(home) + 1);
			ft_strlcat(expanded_path, path + 1, ft_strlen(expanded_path)
				+ ft_strlen(path));
			return (expanded_path);
		}
	}
	return (NULL);
}
