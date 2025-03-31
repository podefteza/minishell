/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:59:20 by carlos-j          #+#    #+#             */
/*   Updated: 2025/03/28 14:42:15 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Helper function to check if a token is a redirection
int is_redirection_token(char *token)
{
    return ((ft_strncmp(token, "<", 1) == 0 && ft_strlen(token) == 1) ||
            (ft_strncmp(token, "<<", 2) == 0 && ft_strlen(token) == 2) ||
            (ft_strncmp(token, ">", 1) == 0 && ft_strlen(token) == 1) ||
            (ft_strncmp(token, ">>", 2) == 0 && ft_strlen(token) == 2));
}


int builtin_echo(char **args, t_shell *shell)
{
    int i;
    int newline;
    int original_stdout;
    int original_stdin;  // Save original stdin

    // Save original STDOUT and STDIN
    original_stdout = dup(STDOUT_FILENO);
    original_stdin = dup(STDIN_FILENO);
    if (original_stdout == -1 || original_stdin == -1)
    {
        perror("dup");
        return 1;  // Return an error code on failure
    }

    // Handle redirections (which might modify STDIN or STDOUT)
    if (handle_redirections(args, shell) == -1)
    {
        dup2(original_stdout, STDOUT_FILENO);
        dup2(original_stdin, STDIN_FILENO);  // Restore original STDIN
        close(original_stdout);
        close(original_stdin);
        return 1;  // Return an error code if redirection fails
    }

    // Process echo arguments
    i = 1;
    newline = 1;
    if (args[i] && ft_strncmp(args[i], "-n", 3) == 0)
    {
        newline = 0;
        i++;
    }

    while (args[i])
    {
        if ((ft_strncmp(args[i], "<", ft_strlen(args[i])) == 0) ||
            (ft_strncmp(args[i], "<<", ft_strlen(args[i])) == 0) ||
            (ft_strncmp(args[i], ">", ft_strlen(args[i])) == 0) ||
            (ft_strncmp(args[i], ">>", ft_strlen(args[i])) == 0))
        {
            i += (args[i + 1]) ? 2 : 1;
            continue;
        }
        printf("%s", args[i]);
        if (args[i + 1])
            printf(" ");
        i++;
    }

    if (newline)
        printf("\n");

    // Restore original STDOUT and STDIN
    dup2(original_stdout, STDOUT_FILENO);
    dup2(original_stdin, STDIN_FILENO);
    close(original_stdout);
    close(original_stdin);

    return 0;  // Return success
}






void	dash_n_option(char **args, char *message)
{
	while (*message == ' ' || *message == '\t')  // Trim leading spaces
		message++;

	if (ft_strncmp(message, "-n", 2) == 0 && (message[2] == ' '
			|| message[2] == '\t' || message[2] == '\0'))
	{
		args[1] = ft_strdup("-n");
		message += 2;
		while (*message == ' ' || *message == '\t')  // Trim spaces again
			message++;
		if (*message)  // Only assign if there's something left
			args[2] = ft_strdup(message);
		else
			args[2] = NULL;
		args[3] = NULL;
	}
	else
	{
		if (*message)  // Avoid creating an empty argument!
			args[1] = ft_strdup(message);
		else
			args[1] = NULL;
		args[2] = NULL;
	}
}

char *get_next_token_for_echo(char **str)
{
    char *start;
    char *token;
    int in_single_quote;
    int in_double_quote;
    int token_len;

    if (!str || !*str || **str == '\0')
        return NULL;

    // Skip leading whitespace
    while (**str == ' ' || **str == '\t')
        (*str)++;

    // If we've reached the end of the string
    if (**str == '\0')
        return NULL;

    start = *str;
    in_single_quote = 0;
    in_double_quote = 0;
    token_len = 0;

    // Iterate through the string to find token boundaries
    while (**str != '\0')
    {
        // Handle quote state changes
        if (**str == '\'' && !in_double_quote)
            in_single_quote = !in_single_quote;
        else if (**str == '"' && !in_single_quote)
            in_double_quote = !in_double_quote;

        // If not in quotes and we hit a space, break
        if (!in_single_quote && !in_double_quote &&
            (**str == ' ' || **str == '\t'))
            break;

        (*str)++;
        token_len++;
    }

    // Allocate and copy the token
    token = malloc(token_len + 1);
    if (!token)
        return NULL;

    ft_strlcpy(token, start, token_len + 1);

    // Skip any trailing whitespace
    while (**str == ' ' || **str == '\t')
        (*str)++;

    return token;
}

char **handle_echo(char *modified_input, t_shell *shell)
{
    char **args = NULL;
    char *message;
    int arg_count = 0;
    char *token;
    char *tmp_input;

    // Create a temporary copy of the input to preserve original
    tmp_input = ft_strdup(modified_input + 4);
    if (!tmp_input)
        return NULL;

    // Remove leading whitespace
    message = tmp_input;
    while (*message == ' ' || *message == '\t')
        message++;

    // First pass: count tokens
    char *count_message = message;
    arg_count = 1;  // Start with "echo"
    while ((token = get_next_token_for_echo(&count_message)))
    {
        arg_count++;
        free(token);
    }

    // Allocate array with exact number of tokens
    args = malloc(sizeof(char *) * (arg_count + 1));
    if (!args)
    {
        free(tmp_input);
        return NULL;
    }

    // Reset for second pass
    args[0] = ft_strdup("echo");
    int i = 1;

    // Second pass: populate arguments
    message = tmp_input;
    while (*message == ' ' || *message == '\t')
        message++;

    while ((token = get_next_token_for_echo(&message)))
    {
        char *cleaned_token = ft_strdup(token);
        if (cleaned_token && cleaned_token[0] != '\'')
            cleaned_token = expand_variables(cleaned_token, shell);

        cleaned_token = handle_quotes(cleaned_token);
        args[i++] = cleaned_token;
        free(token);
    }

    args[i] = NULL;
    free(tmp_input);

    return args;
}
