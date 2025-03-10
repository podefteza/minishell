/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:59:20 by carlos-j          #+#    #+#             */
/*   Updated: 2025/03/10 13:17:10 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	builtin_echo(char **args, int *exit_status)
{
	int	i;
	int	newline;

	i = 1;
	newline = TRUE;
	if (args[i] && ft_strncmp(args[i], "-n", 3) == 0)
	{
		newline = FALSE;
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	*exit_status = 0;
}

void	dash_n_option(char **args, char *message)
{
	if (ft_strncmp(message, "-n", 2) == 0 && (message[2] == ' '
			|| message[2] == '\t' || message[2] == '\0'))
	{
		args[1] = ft_strdup("-n");
		message += 2;
		while (*message && (*message == ' ' || *message == '\t'))
			message++;
		args[2] = ft_strdup(message);
		args[3] = NULL;
	}
	else
	{
		args[1] = ft_strdup(message);
		args[2] = NULL;
	}
}

char	**handle_echo(char *modified_input)
{
	char	**args;
	char	*message;
	int		i;
	char	*cleaned_arg;

	args = malloc(sizeof(char *) * 4);
	if (!args)
		return (NULL);
	args[0] = ft_strdup("echo");
	message = modified_input + 4;
	while (*message && (*message == ' ' || *message == '\t'))
		message++;
	dash_n_option(args, message);
	i = 0;
	while (args[i])
	{
		cleaned_arg = handle_quotes(args[i]);
		free(args[i]);
		args[i] = cleaned_arg;
		i++;
	}
	return (args);
}
