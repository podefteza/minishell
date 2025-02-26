/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 13:55:32 by carlos-j          #+#    #+#             */
/*   Updated: 2025/02/26 14:30:08 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_quotes(char *input)
{
	int		i;
	int		j;
	int		in_single_quote;
	int		in_double_quote;
	int		is_escaped;
	char	*output;

	i = 0;
	j = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	is_escaped = 0;
	output = malloc(ft_strlen(input) + 1);
	if (!output)
		return (NULL);
	while (input[i])
	{
		if (is_escaped)
		{
			output[j++] = input[i];
			is_escaped = 0;
		}
		else if (input[i] == '\\' && !in_single_quote)
			is_escaped = 1;
		else if (input[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (input[i] == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else
			output[j++] = input[i];
		i++;
	}
	output[j] = '\0';
	return (output);
}

int	count_words(char *input)
{
	int	count;
	int	in_single_quote;
	int	in_double_quote;

	count = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (*input)
	{
		while (*input == ' ' && !in_single_quote && !in_double_quote)
			input++;
		if (!*input)
			break ;
		count++;
		while (*input)
		{
			if (*input == '\'' && !in_double_quote)
				in_single_quote = !in_single_quote;
			else if (*input == '\"' && !in_single_quote)
				in_double_quote = !in_double_quote;
			else if (*input == ' ' && !in_single_quote && !in_double_quote)
				break ;
			input++;
		}
	}
	return (count);
}

char	**split_arguments(char *input)
{
	int		i;
	int		in_single_quote;
	int		in_double_quote;
	int		arg_count;
	char	**args;
	char	*current_arg;

	in_single_quote = 0;
	in_double_quote = 0;
	arg_count = 0;
	args = malloc(sizeof(char *) * (count_words(input) + 1));
	if (!args)
		return (NULL);

	while (*input)
	{
		// Skip spaces before the first argument (except inside quotes)
		while (*input == ' ' && !in_single_quote && !in_double_quote)
			input++;
		if (!*input)
			break ;

		current_arg = malloc(ft_strlen(input) + 1);
		if (!current_arg)
			return (free_args(args), NULL);

		i = 0;
		while (*input)
		{
			if (*input == '\'' && !in_double_quote)
				in_single_quote = !in_single_quote;
			else if (*input == '\"' && !in_single_quote)
				in_double_quote = !in_double_quote;
			else if (*input == ' ' && !in_single_quote && !in_double_quote)
			{
				// If we reach a space outside quotes, check next char
				char *next = input + 1;
				while (*next == ' ') // Skip multiple spaces
					next++;
				// If next char is a quote, continue to preserve spaces inside
				if (*next == '\'' || *next == '\"')
					current_arg[i++] = *input;
				else
					break;
			}
			else
				current_arg[i++] = *input;
			input++;
		}
		current_arg[i] = '\0';
		args[arg_count++] = current_arg;
	}
	args[arg_count] = NULL;
	return (args);
}


void	handle_input(char *input, char **envp)
{
	t_builtin	builtins[8];
	char		**commands;
	char		**args;
	int			i;
	int			pipe_fds[2];
	char		*modified_input;

	// Expand variables first (before splitting)
	if (ft_strchr(input, '$'))
		modified_input = expand_variables(input, envp);
	else
		modified_input = ft_strdup(input);
	if (!modified_input)
		return ;

	// Process builtins and commands
	builtin_setup(builtins);

	// Special handling for "echo"
	if (ft_strnstr(modified_input, "echo", ft_strlen(modified_input)))
	{
		args = malloc(sizeof(char *) * 3);
		if (!args)
		{
			free(modified_input);
			return ;
		}
		args[0] = ft_strdup("echo");

		// Move past "echo" and trim leading spaces **before** quotes
		char *message = modified_input + 4;
		while (*message && (*message == ' ' || *message == '\t'))
			message++;

		// Preserve spaces inside quotes
		args[1] = ft_strdup(message);
		args[2] = NULL;

		// **NOW remove quotes, so spaces inside are preserved** ---------------> remove if not working
		i = 0;
		while (args[i])
		{
			char *cleaned_arg = handle_quotes(args[i]);
			free(args[i]);
			args[i] = cleaned_arg;
			i++;
		}
	}
	else
	{
		// **Only split arguments first, handle quotes later**
		args = split_arguments(modified_input);
		free(modified_input);
		if (!args || !args[0])
		{
			free_args(args);
			return ;
		}

		// **NOW remove quotes, so spaces inside are preserved**
		i = 0;
		while (args[i])
		{
			char *cleaned_arg = handle_quotes(args[i]);
			free(args[i]);
			args[i] = cleaned_arg;
			i++;
		}
	}

	// Handle pipes
	if (ft_strchr(args[0], '|'))
	{
		commands = ft_split(args[0], '|');
		if (!commands || !commands[0])
		{
			printf("minishell: syntax error: unexpected '|'\n");
			free_commands(commands);
			free_args(args);
			return ;
		}
		fork_processes(pipe_fds, commands, envp);
		free_commands(commands);
		free_args(args);
		return ;
	}

	// Execute builtins if match
	i = 0;
	while (builtins[i].cmd != NULL)
	{
		if (ft_strncmp(args[0], builtins[i].cmd, ft_strlen(builtins[i].cmd)) == 0)
		{
			builtins[i].func(args);
			free_args(args);
			return ;
		}
		i++;
	}

	// Execute external command
	execute_command(args, envp);
	free_args(args);
}

