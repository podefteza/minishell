/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 16:28:11 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/03 17:21:11 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	print_export_error(char *arg, t_shell *shell)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	shell->exit_status = 1;
}

void	swap_env_vars(char **envp, int i, int *swapped)
{
	char	*temp;

	temp = envp[i];
	envp[i] = envp[i + 1];
	envp[i + 1] = temp;
	*swapped = TRUE;
}

void	sort_env(char **envp)
{
	int		swapped;
	int		i;
	size_t	len1;
	size_t	len2;

	swapped = TRUE;
	while (swapped)
	{
		swapped = FALSE;
		i = 0;
		while (envp[i + 1])
		{
			len1 = ft_strlen(envp[i]);
			len2 = ft_strlen(envp[i + 1]);
			if (len1 > len2 && ft_strncmp(envp[i], envp[i + 1], len1) > 0)
				swap_env_vars(envp, i, &swapped);
			else if (len2 >= len1 && ft_strncmp(envp[i], envp[i + 1], len2) > 0)
				swap_env_vars(envp, i, &swapped);
			i++;
		}
	}
}
