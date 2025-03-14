/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 11:22:45 by carlos-j          #+#    #+#             */
/*   Updated: 2025/03/14 16:31:19 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_unset_error(char *arg, t_shell *shell)
{
	ft_putstr_fd("minishell: unset: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	shell->exit_status = 1;
}

char	**create_new_env(t_shell *shell, int env_index, int env_count)
{
	char	**new_envp;
	int		j;
	int		k;

	new_envp = malloc(env_count * sizeof(char *));
	if (!new_envp)
	{
		ft_putstr_fd("minishell: malloc failed\n", 2);
		shell->exit_status = 1;
		return (NULL);
	}
	j = 0;
	k = -1;
	while (shell->envp[++k])
	{
		if (k != env_index)
			new_envp[j++] = shell->envp[k];
		else
			free(shell->envp[k]);
	}
	new_envp[j] = NULL;
	return (new_envp);
}

void	remove_env_var(t_shell *shell, char *var_name)
{
	int		env_index;
	char	**new_envp;
	int		env_count;

	env_index = find_env_var(shell, var_name);
	if (env_index != -1)
	{
		env_count = 0;
		while (shell->envp[env_count])
			env_count++;
		new_envp = create_new_env(shell, env_index, env_count);
		if (!new_envp)
			return ;
		free(shell->envp);
		shell->envp = new_envp;
	}
}

void	process_unset_arg(char *arg, t_shell *shell)
{
	if (!is_valid_identifier(arg))
		print_unset_error(arg, shell);
	else
		remove_env_var(shell, arg);
}

void	builtin_unset(char **args, t_shell *shell)
{
	int	i;

	if (!args[1])
	{
		shell->exit_status = 0;
		return ;
	}
	i = 1;
	while (args[i])
	{
		process_unset_arg(args[i], shell);
		i++;
	}
	if (shell->exit_status != 1)
		shell->exit_status = 0;
}
