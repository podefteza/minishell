/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 11:22:45 by carlos-j          #+#    #+#             */
/*   Updated: 2025/03/14 11:31:04 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	builtin_unset(char **args, t_shell *shell)
{
	int		i;
	int		j;
	int		env_index;
	char	**new_envp;
	int		env_count;
	int		k;

	if (!args[1])
	{
		shell->exit_status = 0;
		return ;
	}
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			shell->exit_status = 1;
		}
		else
		{
			env_index = find_env_var(shell, args[i]);
			if (env_index != -1)
			{
				env_count = 0;
				while (shell->envp[env_count])
					env_count++;
				new_envp = malloc(env_count * sizeof(char *));
				if (!new_envp)
				{
					ft_putstr_fd("minishell: malloc failed\n", 2);
					shell->exit_status = 1;
					return ;
				}
				j = 0;
				k = 0;
				while (shell->envp[k])
				{
					if (k != env_index)
					{
						new_envp[j] = shell->envp[k];
						j++;
					}
					else
						free(shell->envp[k]);
					k++;
				}
				new_envp[j] = NULL;
				free(shell->envp);
				shell->envp = new_envp;
			}
		}
		i++;
	}
	if (shell->exit_status != 1)
		shell->exit_status = 0;
}
