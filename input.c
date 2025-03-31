/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 13:55:32 by carlos-j          #+#    #+#             */
/*   Updated: 2025/03/31 11:47:52 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_echo_command(char *cmd)
{
	return (ft_strncmp(cmd, "echo", 4) == 0);
}

int	execute_builtin(char **args, t_shell *shell)
{
	int	i;

	i = 0;
	if (!args || !args[0] || !shell->builtins || !shell)
		return (0);
	while (shell->builtins[i].cmd != NULL)
	{
		if (shell->builtins[i].cmd && ft_strncmp(args[0],
				shell->builtins[i].cmd, ft_strlen(shell->builtins[i].cmd)
				+ 1) == 0)
		{
			if (shell->builtins[i].func)
			{
				shell->exit_status = shell->builtins[i].func(args, shell);
				return (1);
			}
		}
		i++;
	}
	return (0);
}

void	handle_input(char *input, t_shell *shell)
{
	char	**commands;
	char	**args;
	char	*modified_input;
	char	*cleaned_arg;
	char	*trimmed_input;
	char	*trimmed;
	char	**raw_args;
	char	**echo_args;
	int		echo_len;
	int		raw_len;
	char	**final_args;
	int		i;
	int		j;

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
		modified_input = ft_strdup(input);
	if (!modified_input)
		return ;
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
		raw_args = split_arguments(modified_input);
		if (!raw_args)
		{
			free(modified_input);
			return ;
		}
		echo_args = handle_echo(raw_args[0], shell);
		echo_len = 0;
		raw_len = 0;
		while (echo_args[echo_len])
			echo_len++;
		while (raw_args[raw_len])
			raw_len++;
		final_args = malloc(sizeof(char *) * (echo_len + raw_len + 1));
		if (!final_args)
		{
			free_array(echo_args);
			free_array(raw_args);
			free(modified_input);
			return ;
		}
		i = 0;
		while (i < echo_len)
		{
			final_args[i] = ft_strdup(echo_args[i]);
			i++;
		}
		j = 1;
		while (raw_args[j])
		{
			final_args[i] = ft_strdup(raw_args[j]);
			i++;
			j++;
		}
		final_args[i] = NULL;
		free_array(echo_args);
		free_array(raw_args);
		free(modified_input);
		args = final_args;
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
		if (is_echo_command(args[0]))
		{
			i = 0;
			while (args[i])
			{
				cleaned_arg = handle_quotes(args[i]);
				free(args[i]);
				args[i] = cleaned_arg;
				i++;
			}
		}
	}
	i = 0;
	while (args[i])
	{
		cleaned_arg = handle_quotes(args[i]);
		args[i] = cleaned_arg;
		i++;
	}
	if (execute_builtin(args, shell))
		return ;
	execute_command(args, shell);
	free_array(args);
}
