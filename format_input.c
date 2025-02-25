/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 13:55:32 by carlos-j          #+#    #+#             */
/*   Updated: 2025/02/25 13:06:20 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	trim_commands(char **commands)
{
	int		i;
	char	*trimmed;

	i = 0;
	while (commands[i])
	{
		trimmed = ft_strtrim(commands[i], " \t\n");
		free(commands[i]);
		commands[i] = trimmed;
		i++;
	}
}

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

int	ft_count_words(char *input)
{
	int		count;
	int		in_single_quote;
	int		in_double_quote;

	count = 0;
	in_single_quote = 0;
	in_double_quote = 0;

	while (*input)
	{
		// Skip leading spaces outside of quotes
		while (*input == ' ' && !in_single_quote && !in_double_quote)
			input++;
		if (!*input)
			break;

		count++; // Found a new word

		// Process word until next space (or end)
		while (*input)
		{
			if (*input == '\'' && !in_double_quote)
				in_single_quote = !in_single_quote;
			else if (*input == '\"' && !in_single_quote)
				in_double_quote = !in_double_quote;
			else if (*input == ' ' && !in_single_quote && !in_double_quote)
				break;
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

	i = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	arg_count = 0;
	args = malloc(sizeof(char *) * (ft_count_words(input) + 1)); // Needs implementation
	if (!args)
		return (NULL);

	while (*input)
	{
		while (*input == ' ' && !in_single_quote && !in_double_quote)
			input++; // Skip unquoted spaces

		if (!*input)
			break;

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
				break; // End of current argument
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

void handle_input(char *input, char **envp)
{
    t_builtin builtins[8];
    char **commands;
    char **args;
    int i;
    int pipe_fds[2];
    char *modified_input;
    char *cleaned_input;

    if (ft_strchr(input, '$'))
        modified_input = expand_variables(input, envp);
    else
        modified_input = ft_strdup(input);

    if (!modified_input)
        return;

    cleaned_input = handle_quotes(modified_input);
    free(modified_input);
    if (!cleaned_input)
        return;

    builtin_setup(builtins);
    if (ft_strchr(cleaned_input, '|'))
    {
        commands = ft_split(cleaned_input, '|');
        if (!commands || !commands[0])
        {
            printf("minishell: syntax error: unexpected '|'\n");
            free_commands(commands);
            free(cleaned_input);
            return;
        }
        fork_processes(pipe_fds, commands, envp);
        free_commands(commands);
        free(cleaned_input);
        return;
    }

    // Use `split_arguments()` instead of `ft_split(cleaned_input, ' ')`
    args = split_arguments(cleaned_input);
    if (!args || !args[0])
    {
        free_args(args);
        free(cleaned_input);
        return;
    }

    i = 0;
    while (builtins[i].cmd != NULL)
    {
        if (ft_strncmp(args[0], builtins[i].cmd, ft_strlen(builtins[i].cmd)) == 0)
        {
            builtins[i].func(args);
            free_args(args);
            free(cleaned_input);
            return;
        }
        i++;
    }

    execute_command(args, envp);
    free_args(args);
    free(cleaned_input);
}
