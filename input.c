/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 13:55:32 by carlos-j          #+#    #+#             */
/*   Updated: 2025/03/21 16:59:21 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_input(char *input, t_shell *shell)
{
	t_builtin	builtins[8];
	char		**commands;
	char		**args;
	int			i;
	char		*modified_input;
	char		*cleaned_arg;
	char		*trimmed_input;
	char		*trimmed;

	modified_input = NULL;
	trimmed_input = ft_strtrim(input, " \t");
	if (trimmed_input[0] == '\0')
	{
		free(trimmed_input);
		return ;
	}
	free(trimmed_input);
	if (count_quotes(input))
		return ;
	if (ft_strchr(input, '$'))
	{
		modified_input = expand_variables(input, shell);
		if (!modified_input)
		{
			modified_input = ft_strdup("");
			if (!modified_input)
				return ;
		}
		else
		{
			trimmed = ft_strtrim(modified_input, " ");
			free(modified_input);
			modified_input = trimmed;
		}
	}
	else
	{
		modified_input = ft_strdup(input);
	}
	if (!modified_input)
		return ;
	builtin_setup(builtins);
	if (ft_strchr(modified_input, '|')
		&& is_pipe_outside_quotes(modified_input))
	{
		commands = split_pipe(modified_input);
		free(modified_input);
		if (!commands)
		{
			free(modified_input);
			return ;
		}
		execute_pipeline(commands, shell);
		free_array(commands);
		return ;
	}
	if (ft_strnstr(modified_input, "echo", ft_strlen(modified_input)))
	{
		args = handle_echo(modified_input);
		free(modified_input);
		if (!args)
			return ;
	}
	else
	{
		args = split_arguments(modified_input);
		free(modified_input);
		if (!args || !args[0])
		{
			free_array(args);
			return ;
		}
		i = 0;
		while (args[i])
		{
			cleaned_arg = handle_quotes(args[i]);
			free(args[i]);
			args[i] = cleaned_arg;
			i++;
		}
	}

	/*// print args >>>>>>>>>>> modify from here to split args if the first or last characters on them are < << > >>
	i = 0;
	while (args[i])
	{
		printf("cmd %i %s\n", i, args[i]);
		i++;
	}*/


	i = 0;
	while (builtins[i].cmd != NULL)
	{
		if (ft_strncmp(args[0], builtins[i].cmd,
				ft_strlen(builtins[i].cmd)) == 0)
		{
			builtins[i].func(args, shell);
			free_array(args);
			return ;
		}
		i++;
	}
	execute_command(args, shell);
	free_array(args);
}
