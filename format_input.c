/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 13:55:32 by carlos-j          #+#    #+#             */
/*   Updated: 2025/03/14 15:25:35 by carlos-j         ###   ########.fr       */
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
	int			in_squote = 0, in_dquote;
	int			i;

	in_squote = 0, in_dquote = 0;
	i = 0;
	if (input)
		str = input;
	if (!str || *str == '\0')
		return (NULL);
	token = malloc(ft_strlen(str) + 1);
	if (!token)
		return (NULL);
	while (*str && (*str == ' ' || *str == '\t'))
		str++;
	while (*str)
	{
		if (*str == '\'' && !in_dquote)
			in_squote = !in_squote;
		else if (*str == '\"' && !in_squote)
			in_dquote = !in_dquote;
		else if (*str == ' ' && !in_squote && !in_dquote)
			break ;
		else
			token[i++] = *str;
		str++;
	}
	token[i] = '\0';
	while (*str == ' ')
		str++;
	if (!*str)
		str = NULL;
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
	token = get_next_token(input); // Custom tokenizer function
	while (token)
	{
		args[i++] = ft_strdup(token); // Duplicate token
		free(token);                  // Free token after copying its contents
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

char	**split_pipe(char *input)
{
	char	*temp;
	int		has_trailing_pipe;

	temp = input;
	has_trailing_pipe = 0;
	while (*temp)
		temp++;
	temp--;
	while (temp >= input && (*temp == ' ' || *temp == '\t'))
		temp--;
	if (temp >= input && *temp == '|')
		has_trailing_pipe = 1;
	if (has_trailing_pipe)
	{
		printf("minishell: syntax error: unexpected '|'\n");
		return (NULL);
	}
	return (ft_split(input, '|'));
}

void	execute_pipeline(char **commands, t_shell *shell)
{
	int		pipe_fds[2];
	int		input_fd;
	pid_t	pid;
	int		i;
	char	**args;

	input_fd = STDIN_FILENO;
	i = 0;
	args = split_arguments(commands[0]);

	// Check for specific conditions before proceeding
	if (args && args[0] && ft_strncmp(args[0], "cat", 4) == 0 && args[1])
	{
		if (access(args[1], F_OK) == -1)
		{
			ft_putstr_fd("cat: ", STDERR_FILENO);
			ft_putstr_fd(args[1], STDERR_FILENO);
			ft_putstr_fd(": ", STDERR_FILENO);
			perror("");
			free_array(args);
			shell->exit_status = 1;
			return ;
		}
	}

	// Handle the 'export' command early
	if (args && args[0] && ft_strncmp(args[0], "export", 7) == 0)
	{
		free_array(args);
		shell->exit_status = 1;
		return;
	}

	// Free arguments after the initial checks
	free_array(args);

	// Start processing the pipeline
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
			// Redirect input if necessary
			if (input_fd != STDIN_FILENO)
			{
				dup2(input_fd, STDIN_FILENO);
				close(input_fd);
			}

			// Redirect output if this is not the last command in the pipeline
			if (commands[i + 1] != NULL)
			{
				dup2(pipe_fds[1], STDOUT_FILENO);
				close(pipe_fds[1]);
			}

			// Split arguments and check if the command is "env"
			args = split_arguments(commands[i]);
			if (args && args[0])
			{
				// Check if the command is "env" and call the builtin function instead of execvp
				if (ft_strncmp(args[0], "env", 4) == 0)
				{
					builtin_env(args, shell);  // Call the builtin_env function
					exit(EXIT_SUCCESS);  // Exit the child process after calling the builtin
				}
				// If not "env", proceed to execute the command normally
				execvp(args[0], args);
				ft_putstr_fd(args[0], STDERR_FILENO);
				ft_putstr_fd(": ", STDERR_FILENO);
				perror("");
			}
			exit(EXIT_FAILURE);
		}
		else
		{
			// Close pipes in the parent process
			if (commands[i + 1] != NULL)
				close(pipe_fds[1]);
			if (input_fd != STDIN_FILENO)
				close(input_fd);

			// Update input_fd to the pipe read end for the next iteration
			input_fd = pipe_fds[0];
		}
		i++;
	}

	// Wait for all child processes to finish
	while (wait(NULL) > 0)
		;
}


void handle_input(char *input, t_shell *shell)
{
    t_builtin    builtins[8];
    char        **commands;
    char        **args;
    int         i;
    char        *modified_input = NULL;
    char        *cleaned_arg;
    char        *trimmed_input;
    char        *trimmed;

    // Check if input is empty
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
            modified_input = ft_strdup(""); // Handle case when expansion fails
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

    builtin_setup(builtins); // Move this to shell setup if necessary

    // Handle pipeline case
    if (ft_strchr(modified_input, '|') && is_pipe_outside_quotes(modified_input))
    {
        commands = split_pipe(modified_input);
        free(modified_input);  // Freeing modified_input before using it for pipeline
        if (!commands)
        {
            free(modified_input);  // Just in case commands is NULL
            return ;
        }
        execute_pipeline(commands, shell);
        free_array(commands);
        return ;
    }

    // Handle echo
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

    // Handle built-in commands
    i = 0;
    while (builtins[i].cmd != NULL)
    {
        if (ft_strncmp(args[0], builtins[i].cmd, ft_strlen(builtins[i].cmd)) == 0)
        {
            builtins[i].func(args, shell);
            free_array(args);
            return ;
        }
        i++;
    }

    // Execute external command
    execute_command(args, shell);
    free_array(args);
}

