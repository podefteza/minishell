/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 13:55:32 by carlos-j          #+#    #+#             */
/*   Updated: 2025/03/10 16:56:00 by carlos-j         ###   ########.fr       */
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

char	*get_next_token(char *input)
{
	static char	*str;
	char		*token;
	int			in_squote = 0, in_dquote = 0;
	int			i = 0;

	if (input)
		str = input;
	if (!str || *str == '\0')
		return (NULL);

	token = malloc(ft_strlen(str) + 1);
	if (!token)
		return (NULL);

	while (*str && (*str == ' ' || *str == '\t'))
		str++; // Skip leading spaces

	while (*str)
	{
		if (*str == '\'' && !in_dquote)
			in_squote = !in_squote;
		else if (*str == '\"' && !in_squote)
			in_dquote = !in_dquote;
		else if (*str == ' ' && !in_squote && !in_dquote)
			break; // End token on space outside quotes
		else
			token[i++] = *str;
		str++;
	}
	token[i] = '\0';

	while (*str == ' ') // Move to next token
		str++;

	return (token);
}


char	**split_arguments(char *input)
{
	int		arg_count;
	char	**args;
	char	*token;
	int		i;

	arg_count = count_words(input);
	args = malloc(sizeof(char *) * (arg_count + 1));
	if (!args)
		return (NULL);

	i = 0;
	token = get_next_token(input);  // Custom tokenizer function
	while (token)
	{
		args[i++] = ft_strdup(token);
		token = get_next_token(NULL);
	}
	args[i] = NULL;
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

char **split_pipe(char *input)
{
    char *temp = input;
    int has_trailing_pipe = 0;

    while (*temp)
        temp++;

    temp--;
    while (temp >= input && (*temp == ' ' || *temp == '\t'))
        temp--;

    if (temp >= input && *temp == '|')
        has_trailing_pipe = 1;
    if (has_trailing_pipe) {
        printf("minishell: syntax error: unexpected '|'\n");
        return NULL;
    }
    return (ft_split(input, '|'));
}

void	execute_pipeline(char **commands, int *exit_status)
{
	int		pipe_fds[2];
	int		input_fd;
	pid_t	pid;
	int		i;
	char	**args;

	input_fd = STDIN_FILENO;
	i = 0;

	args = split_arguments(commands[0]);
	if (args && args[0] && ft_strncmp(args[0], "cat", 4) == 0 && args[1])
	{
		if (access(args[1], F_OK) == -1)
		{
			ft_putstr_fd("cat: ", STDERR_FILENO);
			ft_putstr_fd(args[1], STDERR_FILENO);
			ft_putstr_fd(": ", STDERR_FILENO);
			perror("");
			free_args(args);
			*exit_status = 1;
			return ;
		}
	}
	free_args(args);
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
				ft_putstr_fd(args[0], STDERR_FILENO);
				ft_putstr_fd(": ", STDERR_FILENO);
				perror("");
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


void	handle_input(char *input, char **envp, int *exit_status, pid_t *last_bg_pid)
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
	{
		modified_input = expand_variables(input, envp, exit_status, last_bg_pid);
		if (!modified_input)
		{
			modified_input = ft_strdup("");
			if (!modified_input)
				return;
		}
		else
		{
			char *trimmed = ft_strtrim(modified_input, " ");
			free(modified_input);
			modified_input = trimmed;
		}
	}
	else
		modified_input = ft_strdup(input);
	if (!modified_input)
		return ;
	builtin_setup(builtins);
	if (ft_strchr(modified_input, '|')
		&& is_pipe_outside_quotes(modified_input))
	{
		commands = split_pipe(modified_input);
		if (!commands)
		{
			free(modified_input);
			return;
		}
		execute_pipeline(commands, exit_status);
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
			return;
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
			builtins[i].func(args, exit_status);
			free_args(args);
			return ;
		}
		i++;
	}
	execute_command(args, envp, exit_status, last_bg_pid);
	free_args(args);
}
