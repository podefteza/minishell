/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:29:25 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/28 13:32:41 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	init_tokenizer(t_tokenizer *t, const char *input, t_shell *shell)
{
	t->input = input;
	t->pos = 0;
	t->done = FALSE;
	t->in_quotes = FALSE;
	t->quote_char = 0;
	t->shell = shell;
}

static char *collect_heredoc_content(char *delimiter, int expand, t_shell *shell)
{
    char *line;
    char *expanded_line;
    char *content = ft_strdup("");
    char *temp;
    char *newline_content;

    if (!content)
        return (NULL);

    while (1)
    {
        line = readline("> ");
        if (!line)
            break;

        if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0 &&
            ft_strlen(line) == ft_strlen(delimiter))
        {
            free(line);
            break;
        }

        if (expand)
        {
            expanded_line = expand_variables(line, shell);
            free(line);
            if (!expanded_line)
            {
                free(content);
                return (NULL);
            }
            line = expanded_line;
        }

        // Append line to content
        temp = ft_strjoin(content, line);
        free(content);
        free(line);
        if (!temp)
            return (NULL);

        // Add newline
        newline_content = ft_strjoin(temp, "\n");
        free(temp);
        if (!newline_content)
            return (NULL);

        content = newline_content;
    }

    return (content);
}

// Create a pipe and write heredoc content to it
/*static int create_heredoc_pipe(char *content)
{
    int pipe_fd[2];
    pid_t pid;

    if (pipe(pipe_fd) == -1)
        return (-1);

    pid = fork();
    if (pid == -1)
    {
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        return (-1);
    }

    if (pid == 0) // Child process
    {
        close(pipe_fd[0]); // Close read end
        if (content)
            write(pipe_fd[1], content, ft_strlen(content));
        close(pipe_fd[1]);
        exit(0);
    }
    else // Parent process
    {
        close(pipe_fd[1]); // Close write end
        return (pipe_fd[0]); // Return read end
    }
}*/

static char *create_temp_filename(void)
{
    char *temp_dir = "/tmp/heredoc_";
    char *pid_str = ft_itoa(getpid());
    static int counter = 0;
    char *counter_str = ft_itoa(counter++);
    char *temp_path;
    char *temp_with_pid;
    char *underscore = "_";

    if (!pid_str || !counter_str)
    {
        free(pid_str);
        free(counter_str);
        return (NULL);
    }

    // Build: /tmp/heredoc_[PID]_[COUNTER]
    temp_with_pid = ft_strjoin(temp_dir, pid_str);
    free(pid_str);
    if (!temp_with_pid)
    {
        free(counter_str);
        return (NULL);
    }

    temp_path = ft_strjoin(temp_with_pid, underscore);
    free(temp_with_pid);
    if (!temp_path)
    {
        free(counter_str);
        return (NULL);
    }

    temp_with_pid = ft_strjoin(temp_path, counter_str);
    free(temp_path);
    free(counter_str);

    return (temp_with_pid);
}

static char *handle_heredoc_to_file(char *delimiter, int expand, t_shell *shell)
{
    char *temp_filename;
    int temp_fd;
    char *content;

    // First collect all heredoc content
    content = collect_heredoc_content(delimiter, expand, shell);
    if (!content)
        return (NULL);

    temp_filename = create_temp_filename();
    if (!temp_filename)
    {
        free(content);
        return (NULL);
    }

    // Use 0644 permissions so child processes can read the file
    temp_fd = open(temp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (temp_fd == -1)
    {
        free(temp_filename);
        free(content);
        return (NULL);
    }

    write(temp_fd, content, ft_strlen(content));
    close(temp_fd);
    free(content);

    return (temp_filename);
}


static char **tokenize_command(const char *cmd, t_shell *shell)
{
    t_tokenizer tokenizer;
    t_list *tokens;
    char *token;
    char **result;
    char *delimiter_token;
    char *temp_filename;

    init_tokenizer(&tokenizer, cmd, shell);
    tokens = NULL;
    while (!tokenizer.done)
    {
        token = get_next_token_tokenizer(&tokenizer);
        if (!token && !tokenizer.done)
        {
            ft_lstclear(&tokens, free);
            return (NULL);
        }
        if (token && ft_strncmp(token, "<<", 3) == 0)
        {
            delimiter_token = get_next_token_tokenizer(&tokenizer);
            if (!delimiter_token)
                return (free(token), ft_lstclear(&tokens, free), NULL);

            // Create temporary file with readable permissions
            temp_filename = handle_heredoc_to_file(delimiter_token, 0, shell);
            free(delimiter_token);
            if (!temp_filename)
                return (free(token), ft_lstclear(&tokens, free), NULL);

            free(token);
            token = ft_strdup("<");
            if (!token)
            {
                free(temp_filename);
                return (ft_lstclear(&tokens, free), NULL);
            }
            ft_lstadd_back(&tokens, ft_lstnew(token));
            ft_lstadd_back(&tokens, ft_lstnew(temp_filename));
        }
        else if (token)
            ft_lstadd_back(&tokens, ft_lstnew(token));
    }
    result = list_to_array(tokens);
    ft_lstclear(&tokens, free);
    return (result);
}

static int	is_inside_quotes(const char *str, const char *pos)
{
	const char	*p = str;
	int			in_single;
	int			in_double;

	in_single = 0;
	in_double = 0;
	while (p < pos)
	{
		if (*p == '\'' && !in_double)
			in_single = !in_single;
		else if (*p == '"' && !in_single)
			in_double = !in_double;
		p++;
	}
	return (in_single || in_double);
}

char *preprocess_heredocs(char *input, t_shell *shell)
{
    char *result;
    char *pos;
    char *delim_start;
    char *delim_end;
    int delim_len;
    char *delimiter;
    char *temp_filename;
    int original_len;
    int replacement_len;
    int result_len;
    char *new_result;
    int pos_offset;

    result = ft_strdup(input);
    pos = result;
    while ((pos = ft_strnstr(pos, "<<", ft_strlen(pos))))
    {
        if (is_inside_quotes(result, pos))
        {
            pos += 2;
            continue;
        }
        delim_start = pos + 2;
        while (*delim_start && ft_isspace(*delim_start))
            delim_start++;
        if (!*delim_start)
        {
            pos += 2;
            continue;
        }
        delim_end = delim_start;
        while (*delim_end && !ft_isspace(*delim_end) && *delim_end != '|'
            && *delim_end != '>')
            delim_end++;
        delim_len = delim_end - delim_start;
        int quoted = (*delim_start == '\'' || *delim_start == '"');
        if (quoted)
        {
            delim_start++;
            delim_end--;
            delim_len -= 2;
        }
        if (delim_len <= 0)
        {
            free(result);
            return (NULL);
        }
        delimiter = malloc(delim_len + 1);
        ft_strlcpy(delimiter, delim_start, delim_len + 1);
        delimiter[delim_len] = '\0';

        // Create temporary file with readable permissions
        temp_filename = handle_heredoc_to_file(delimiter, !quoted, shell);
        free(delimiter);
        if (!temp_filename)
        {
            free(result);
            return (NULL);
        }

        // Build "< filename" instead of just "filename"
        char *redirect_and_file = ft_strjoin("< ", temp_filename);
        if (!redirect_and_file)
        {
            free(result);
            free(temp_filename);
            return (NULL);
        }

        original_len = delim_end - pos;
        replacement_len = ft_strlen(redirect_and_file);
        result_len = ft_strlen(result);
        new_result = malloc(result_len - original_len + replacement_len + 1);
        pos_offset = pos - result;
        ft_strlcpy(new_result, result, pos_offset + 1);
        new_result[pos_offset] = '\0';
        ft_strlcat(new_result, redirect_and_file, result_len - original_len
            + replacement_len + 1);
        ft_strlcat(new_result, delim_end, result_len - original_len
            + replacement_len + 1);

        free(redirect_and_file);
        free(result);
        free(temp_filename);
        result = new_result;
        pos = result + pos_offset + replacement_len;
    }
    return (result);
}


static int	count_args(char **args)
{
	int	count;

	count = 0;
	while (args[count])
		count++;
	return (count);
}

static int	fill_commands(char ***commands, char **args, int count,
		t_shell *shell)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (ft_strncmp(args[i], "|", 2) == 0)
		{
			commands[i] = malloc(2 * sizeof(char *));
			if (!commands[i])
				return (-1);
			commands[i][0] = ft_strdup("|");
			commands[i][1] = NULL;
		}
		else
		{
			commands[i] = tokenize_command(args[i], shell);
			if (!commands[i])
				return (i);
		}
		i++;
	}
	return (count);
}

void	split_commands(t_shell *shell)
{
	int	count;
	int	allocated;
	int	j;

	count = count_args(shell->input.args);
	shell->input.commands = malloc((count + 1) * sizeof(char **));
	if (!shell->input.commands)
		return ;
	allocated = fill_commands(shell->input.commands, shell->input.args, count,
			shell);
	if (allocated == -1 || allocated < count)
	{
		j = 0;
		while (shell->input.commands[j])
			free_array(shell->input.commands[j++]);
		free(shell->input.commands);
		shell->input.commands = NULL;
	}
	else
		shell->input.commands[count] = NULL;
}
