/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 02:04:37 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/29 02:44:53 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	heredoc_sigint_handler(int signo)
{
	(void)signo;
	g_signal_status = 1;
	write(STDERR_FILENO, "\n", 1);
}

static char	*collect_heredoc_content(char *delimiter, int expand,
		t_shell *shell)
{
	char				*line;
	char				*expanded_line;
	char				*content;
	char				*temp;
	char				*newline_content;
	struct sigaction	sa;
	struct sigaction	old_sa;

	content = ft_strdup("");
	if (!content)
		return (NULL);
	sa.sa_handler = heredoc_sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, &old_sa);
	g_signal_status = 0;
	while (!g_signal_status)
	{
		line = readline("> ");
		if (!line || g_signal_status)
		{
			if (line)
				free(line);
			break ;
		}
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
			&& ft_strlen(line) == ft_strlen(delimiter))
		{
			free(line);
			break ;
		}
		if (expand)
		{
			expanded_line = expand_variables(line, shell);
			free(line);
			if (!expanded_line)
			{
				free(content);
				content = NULL;
				break ;
			}
			line = expanded_line;
		}
		temp = ft_strjoin(content, line);
		free(content);
		free(line);
		if (!temp)
		{
			content = NULL;
			break ;
		}
		newline_content = ft_strjoin(temp, "\n");
		free(temp);
		if (!newline_content)
		{
			content = NULL;
			break ;
		}
		content = newline_content;
	}
	sigaction(SIGINT, &old_sa, NULL);
	return (content);
}

static char	*create_temp_filename(void)
{
	char		*temp_dir;
	char		*pid_str;
	static int	counter = 0;
	char		*counter_str;
	char		*temp_path;
	char		*temp_with_pid;
	char		*underscore;

	temp_dir = "/tmp/heredoc_";
	pid_str = ft_itoa(getpid());
	counter_str = ft_itoa(counter++);
	underscore = "_";
	if (!pid_str || !counter_str)
	{
		free(pid_str);
		free(counter_str);
		return (NULL);
	}
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

char	*handle_heredoc_to_file(char *delimiter, int expand, t_shell *shell)
{
	char	*temp_filename;
	int		temp_fd;
	char	*content;

	content = collect_heredoc_content(delimiter, expand, shell);
	if (!content)
		return (NULL);
	temp_filename = create_temp_filename();
	if (!temp_filename)
	{
		free(content);
		return (NULL);
	}
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

char	*preprocess_heredocs(char *input, t_shell *shell)
{
	char	*result;
	char	*pos;
	char	*delim_start;
	char	*delim_end;
	int		delim_len;
	char	*delimiter;
	char	*temp_filename;
	int		original_len;
	int		replacement_len;
	int		result_len;
	char	*new_result;
	int		pos_offset;
	int		quoted;
	char	*redirect_and_file;

	result = ft_strdup(input);
	pos = result;
	while ((pos = ft_strnstr(pos, "<<", ft_strlen(pos))))
	{
		if (is_inside_quotes(result, pos))
		{
			pos += 2;
			continue ;
		}
		delim_start = pos + 2;
		while (*delim_start && ft_isspace(*delim_start))
			delim_start++;
		if (!*delim_start)
		{
			pos += 2;
			continue ;
		}
		delim_end = delim_start;
		while (*delim_end && !ft_isspace(*delim_end) && *delim_end != '|'
			&& *delim_end != '>')
			delim_end++;
		delim_len = delim_end - delim_start;
		quoted = (*delim_start == '\'' || *delim_start == '"');
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
		temp_filename = handle_heredoc_to_file(delimiter, !quoted, shell);
		free(delimiter);
		if (!temp_filename)
		{
			free(result);
			return (NULL);
		}
		redirect_and_file = ft_strjoin("< ", temp_filename);
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
