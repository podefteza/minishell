/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 13:55:32 by carlos-j          #+#    #+#             */
/*   Updated: 2025/02/28 17:41:40 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_quotes(char *input)
{
	int	in_single_quote;
	int	in_double_quote;

	in_single_quote = 0;
	in_double_quote = 0;
	while (*input)
	{
		if (*input == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (*input == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		input++;
	}
	if (in_single_quote || in_double_quote)
	{
		printf("minishell: syntax error: unclosed quotes\n");
		return (1);
	}
	return (0);
}

char	*handle_quotes(char *input)
{
	int		i;
	int		j;
	int		in_single_quote;
	int		in_double_quote;
	char	*output;

	i = 0;
	j = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	output = malloc(ft_strlen(input) + 1);
	if (!output)
		return (NULL);
	while (input[i])
	{
		if (input[i] == '\'' && !in_double_quote)
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
	char	*next;

	in_single_quote = 0;
	in_double_quote = 0;
	arg_count = 0;
	args = malloc(sizeof(char *) * (count_words(input) + 1));
	if (!args)
		return (NULL);
	while (*input)
	{
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
				next = input + 1;
				while (*next == ' ')
					next++;
				if (*next == '\'' || *next == '\"')
					current_arg[i++] = *input;
				else
					break ;
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

int	is_pipe_outside_quotes(char *input)
{
	int	in_single_quotes;
	int	in_double_quotes;

	in_single_quotes = 0;
	in_double_quotes = 0;
	while (*input)
	{
		if (*input == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		else if (*input == '"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		else if (*input == '|' && !in_single_quotes && !in_double_quotes)
			return (1);
		input++;
	}
	return (0);
}

char	**split_commands(char *input)
{
	return (ft_split(input, '|'));
}

void	execute_pipeline(char **commands)
{
	int		pipe_fds[2];
	int		input_fd;
	pid_t	pid;
	int		i;
	char	**args;

	input_fd = STDIN_FILENO;
	i = 0;
	while (commands[i] != NULL)
	{
		if (commands[i + 1] != NULL)
		{
			if (pipe(pipe_fds) == -1)
			{
				perror("minishell: pipe");
				return ;
			}
		}
		pid = fork();
		if (pid == -1)
		{
			perror("minishell: fork");
			return ;
		}
		if (pid == 0)
		{
			if (input_fd != STDIN_FILENO)
			{
				dup2(input_fd, STDIN_FILENO);
				close(input_fd);
			}
			if (commands[i + 1] != NULL)
			{
				dup2(pipe_fds[1], STDOUT_FILENO);
				close(pipe_fds[1]);
			}
			args = split_arguments(commands[i]);
			if (args && args[0])
			{
				execvp(args[0], args);
				perror("minishell: execvp");
			}
			exit(EXIT_FAILURE);
		}
		else
		{
			if (commands[i + 1] != NULL)
				close(pipe_fds[1]);
			if (input_fd != STDIN_FILENO)
				close(input_fd);
			input_fd = pipe_fds[0];
		}
		i++;
	}
	while (wait(NULL) > 0)
		;
}

void	handle_input(char *input, char **envp)
{
	t_builtin	builtins[8];
	char		**commands;
	char		**args;
	int			i;
	char		*modified_input;
	char		*cleaned_arg;

	if (count_quotes(input))
		return ;
	if (ft_strchr(input, '$'))
		modified_input = expand_variables(input, envp);
	else
		modified_input = ft_strdup(input);
	if (!modified_input)
		return ;
	builtin_setup(builtins);
	if (ft_strchr(modified_input, '|')
		&& is_pipe_outside_quotes(modified_input))
	{
		commands = split_commands(modified_input);
		if (!commands || !commands[0])
		{
			printf("minishell: syntax error: unexpected '|'\n");
			free(modified_input);
			return ;
		}
		execute_pipeline(commands);
		free_commands(commands);
		free(modified_input);
		return ;
	}
	if (ft_strnstr(modified_input, "echo", ft_strlen(modified_input)))
	{
		args = handle_echo(modified_input);
		if (!args)
		{
			free(modified_input);
			return ;
		}
	}
	else
	{
		args = split_arguments(modified_input);
		free(modified_input);
		if (!args || !args[0])
		{
			free_args(args);
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
	i = 0;
	while (builtins[i].cmd != NULL)
	{
		if (ft_strncmp(args[0], builtins[i].cmd,
				ft_strlen(builtins[i].cmd)) == 0)
		{
			builtins[i].func(args);
			free_args(args);
			return ;
		}
		i++;
	}
	execute_command(args, envp);
	free_args(args);
}
