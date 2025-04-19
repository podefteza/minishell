/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:18:48 by carlos-j          #+#    #+#             */
/*   Updated: 2025/04/19 09:23:52 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	cd_error(t_shell *shell)
{
	shell->exit_status = 1;
	ft_putstr_fd("minishell: cd: too many arguments\n", 2);
}

char	*expand_tilde_unquoted(char *input)
{
	char	*home;
	char	*result;
	int		i;
	int		j;
	int		in_single;
	int		in_double;

	home = getenv("HOME");
	if (!home)
		return (ft_strdup(input));

	result = malloc(ft_strlen(input) + (ft_strlen(home) * 2));
	if (!result)
		return (NULL);

	i = 0;
	j = 0;
	in_single = 0;
	in_double = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (input[i] == '"' && !in_single)
			in_double = !in_double;
		if (input[i] == '~' && !in_single && !in_double && (i == 0 || input[i - 1] == ' '))
		{
			int k = 0;
			while (home[k])
				result[j++] = home[k++];
			i++;
		}
		else
			result[j++] = input[i++];
	}
	result[j] = '\0';
	return (result);
}
