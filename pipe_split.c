/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:58:40 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/16 08:47:51 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	has_trailing_pipe(char *input)
{
	char	*temp;

	temp = input;
	while (*temp)
		temp++;
	temp--;
	while (temp >= input && (*temp == ' ' || *temp == '\t'))
		temp--;
	if (temp >= input && *temp == '|')
		return (1);
	return (0);
}

static int	has_invalid_pipe_syntax(char *input)
{
	char	*trimmed;

	trimmed = input;
	while (*trimmed == ' ' || *trimmed == '\t')
		trimmed++;
	if (*trimmed == '|')
		return (1);
	while (*input)
	{
		if (*input == '<' && *(input + 1) == '|')
			return (1);
		input++;
	}
	return (has_trailing_pipe(input));
}

char **split_pipe(t_shell *shell) {
    if (has_trailing_pipe(shell->input.processed) ||
        has_invalid_pipe_syntax(shell->input.processed) ||
        shell->input.processed[0] == '|') {
        ft_puterr("minishell", SNT, " `|'", "\n");
        shell->exit_status = 2;
        return NULL;
    }

    // First count how many pipes we have
    int pipe_count = 0;
    for (int i = 0; shell->input.processed[i]; i++) {
        if (shell->input.processed[i] == '|' && is_pipe_outside_quotes(shell->input.processed))
            pipe_count++;
    }

    // Allocate space for commands + pipes + NULL terminator
    char **result = malloc((pipe_count * 2 + 1 + 1) * sizeof(char *)); // Extra +1 for safety
    if (!result) return NULL;

    char *start = shell->input.processed;
    int arg_index = 0;
    int in_quotes = 0;
    char quote_char = 0;

    for (char *ptr = shell->input.processed; *ptr; ptr++) {
        if (*ptr == '\'' || *ptr == '"') {
            if (in_quotes && quote_char == *ptr) {
                in_quotes = 0;
            } else if (!in_quotes) {
                in_quotes = 1;
                quote_char = *ptr;
            }
            continue;
        }

        if (!in_quotes && *ptr == '|') {
            // Create temporary substring for the command before pipe
            char *temp = malloc(ptr - start + 1);
            if (!temp) {
                while (arg_index > 0) free(result[--arg_index]);
                free(result);
                return NULL;
            }
            ft_strlcpy(temp, start, ptr - start + 1);

            // Trim and add the command
            char *cmd = ft_strtrim(temp, " ");
            free(temp);
            if (!cmd) {
                while (arg_index > 0) free(result[--arg_index]);
                free(result);
                return NULL;
            }
            result[arg_index++] = cmd;

            // Add the pipe as separate argument
            result[arg_index++] = ft_strdup("|");
            if (!result[arg_index-1]) {
                free(cmd);
                while (arg_index > 1) free(result[--arg_index]);
                free(result);
                return NULL;
            }

            start = ptr + 1;
        }
    }

    // Add the last command
    char *last_cmd = ft_strdup(start);
    if (!last_cmd) {
        while (arg_index > 0) free(result[--arg_index]);
        free(result);
        return NULL;
    }
    char *trimmed_last = ft_strtrim(last_cmd, " ");
    free(last_cmd);
    if (!trimmed_last) {
        while (arg_index > 0) free(result[--arg_index]);
        free(result);
        return NULL;
    }
    result[arg_index++] = trimmed_last;
    result[arg_index] = NULL;

    return result;
}
