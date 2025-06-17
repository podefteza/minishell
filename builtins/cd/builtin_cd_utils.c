/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:18:48 by carlos-j          #+#    #+#             */
/*   Updated: 2025/06/17 13:46:23 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	should_expand_tilde(char *input, int i, int in_single,
		int in_double)
{
	return (!in_single && !in_double && is_tilde_expansion_point(input, i));
}

static int	expand_tilde(char *input, int i, t_shell *shell, int *skip)
{
	int	username_len;

	username_len = get_username_length(input, i + 1);
	if (is_current_user(input, i + 1, username_len, shell))
	{
		*skip = 1 + username_len;
		return (1);
	}
	*skip = 1;
	return (0);
}

static void	expand_loop(char *input, char *result, t_shell *shell)
{
	int	i;
	int	j;
	int	in_single;
	int	in_double;
	int	skip;

	i = 0;
	j = 0;
	in_single = 0;
	in_double = 0;
	while (input[i])
	{
		update_quote_state(input[i], &in_single, &in_double);
		if (should_expand_tilde(input, i, in_single, in_double)
			&& expand_tilde(input, i, shell, &skip))
		{
			j = copy_home(result, j, shell->home);
			i += skip;
		}
		else
			result[j++] = input[i++];
	}
	result[j] = '\0';
}

char	*expand_tilde_unquoted(char *input, t_shell *shell)
{
	char	*result;

	if (!shell->home)
		return (ft_strdup(input));
	result = malloc(ft_strlen(input) + ft_strlen(shell->home) + 1);
	if (!result)
		return (NULL);
	expand_loop(input, result, shell);
	return (result);
}

void	handle_home_directory(t_shell *shell, char *home, char *old_pwd)
{
	char	new_pwd[PATH_MAX];

	if (chdir(home) == 0)
	{
		if (getcwd(new_pwd, sizeof(new_pwd)))
		{
			shell->envp = add_or_update_env(shell, "OLDPWD", old_pwd);
			shell->export_list = add_or_update_export_list(shell->export_list,
					"OLDPWD", old_pwd);
			shell->envp = add_or_update_env(shell, "PWD", new_pwd);
			shell->export_list = add_or_update_export_list(shell->export_list,
					"PWD", new_pwd);
		}
		shell->exit_status = 0;
	}
	else
	{
		shell->exit_status = 1;
		perror("minishell: cd");
	}
}
