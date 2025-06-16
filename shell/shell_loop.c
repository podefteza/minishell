/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:19:12 by carlos-j          #+#    #+#             */
/*   Updated: 2025/06/16 21:41:26 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


// backup after testing...
/*static char	*read_user_input(t_shell *shell, char *display_path, char *prompt)
{
	char	*line;

	build_prompt(prompt, shell, display_path);
	shell->is_prompting = TRUE;
	line = readline(prompt);
	shell->is_prompting = FALSE;
	if (g_signal_status)
	{
		shell->exit_status = 130;
		g_signal_status = 0;
	}
	return (line);
}*/


//////////////////////// START OF THE TEST...
/*# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

static char	*get_line_from_buffer(char **buffer, int *newline_index)
{
	char	*line;
	int		i;
	int		line_length;

	i = 0;
	while ((*buffer)[i] && (*buffer)[i] != '\n')
		i++;
	line_length = i;
	if ((*buffer)[i] == '\n')
		line_length++;
	line = (char *)ft_calloc(line_length + 1, sizeof(char));
	if (line == NULL)
		return (NULL);
	i = 0;
	while (i < line_length)
	{
		line[i] = (*buffer)[i];
		i++;
	}
	line[line_length] = '\0';
	*newline_index = line_length;
	return (line);
}

static void	update_buffer(char **buffer, int newline_index)
{
	char	*new_buffer;
	int		i;
	int		j;

	i = 0;
	j = newline_index;
	while ((*buffer)[j])
	{
		(*buffer)[i] = (*buffer)[j];
		i++;
		j++;
	}
	(*buffer)[i] = '\0';
	new_buffer = (char *)ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (new_buffer == NULL)
		return ;
	i = 0;
	while ((*buffer)[i])
	{
		new_buffer[i] = (*buffer)[i];
		i++;
	}
	free(*buffer);
	*buffer = new_buffer;
}

static int	read_error(char **buffer, char *temp_buf)
{
	free(temp_buf);
	free(*buffer);
	*buffer = NULL;
	return (-1);
}

static int	read_and_append(int fd, char **buffer)
{
	char	*temp_buf;
	int		bytes_read;

	temp_buf = (char *)ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (temp_buf == NULL)
		return (-1);
	while (ft_strchr(*buffer, '\n') == NULL)
	{
		bytes_read = read(fd, temp_buf, BUFFER_SIZE);
		if (bytes_read == -1)
			return (read_error(buffer, temp_buf));
		if (bytes_read == 0)
			break ;
		temp_buf[bytes_read] = '\0';
		*buffer = ft_strjoin(*buffer, temp_buf);
		if (*buffer == NULL)
		{
			free(temp_buf);
			return (-1);
		}
	}
	free(temp_buf);
	return (0);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	int			newline_index;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (buffer == NULL)
	{
		buffer = (char *)ft_calloc(BUFFER_SIZE + 1, sizeof(char));
		if (buffer == NULL)
			return (NULL);
	}
	if (read_and_append(fd, &buffer) == -1)
		return (NULL);
	if (buffer == NULL || buffer[0] == '\0')
	{
		free(buffer);
		buffer = NULL;
		return (NULL);
	}
	line = get_line_from_buffer(&buffer, &newline_index);
	update_buffer(&buffer, newline_index);
	return (line);
}
*/
//////////////////////// END OF THE TEST...

static char	*get_display_path(char *cwd, t_shell *shell)
{
	if (getcwd(cwd, PATH_MAX) == NULL)
	{
		perror("getcwd() error");
		ft_strlcpy(cwd, "unknown", PATH_MAX);
	}
	return (shorten_path(cwd, shell->home));
}

/*
// FOR TESTING........
static char	*read_user_input(t_shell *shell, char *display_path, char *prompt)
{
	char	*line;
	char	*temp;

	if (isatty(fileno(stdin)))
	{
		build_prompt(prompt, shell, display_path);
		shell->is_prompting = TRUE;
		line = readline(prompt);
		shell->is_prompting = FALSE;
	}
	else
	{
		line = get_next_line(fileno(stdin));
		if (!line)
			return (NULL);
		temp = ft_strtrim(line, "\n");
		free(line);
		line = temp;
	}
	if (g_signal_status)
	{
		shell->exit_status = 130;
		g_signal_status = 0;
	}
	return (line);
}*/

static char	*read_user_input(t_shell *shell, char *display_path, char *prompt)
{
	char	*line;

	build_prompt(prompt, shell, display_path);
	shell->is_prompting = TRUE;
	line = readline(prompt);
	shell->is_prompting = FALSE;
	if (g_signal_status)
	{
		shell->exit_status = 130;
		g_signal_status = 0;
	}
	return (line);
}


static void	process_input_line(t_shell *shell)
{
	if (*shell->input.raw)
	{
		add_history(shell->input.raw);
		handle_input(shell);
	}
}

void	run_shell_loop(t_shell *shell)
{
	char	cwd[PATH_MAX];
	char	prompt[PROMPT_MAX];
	char	*display_path;

	while (1)
	{
		display_path = get_display_path(cwd, shell);
		shell->input.raw = read_user_input(shell, display_path, prompt);
		shell->is_prompting = FALSE;
		if (!shell->input.raw)
		{
			shell->should_exit = 1;
			free_shell_resources(shell);
			exit(shell->exit_status);
		}
		else
			process_input_line(shell);
		if (shell->should_exit)
			break ;
	}
	free_shell_resources(shell);
	rl_clear_history();
}
