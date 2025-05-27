/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:29:25 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/27 23:31:38 by carlos-j         ###   ########.fr       */
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

static char	**tokenize_command(const char *cmd, t_shell *shell)
{
	t_tokenizer	tokenizer;
	t_list		*tokens;
	char		*token;
	char		**result;
	char		*delimiter_token;
	int			heredoc_fd;
				char fd_str[32];

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
		if (token)
		{
			if (ft_strncmp(token, "<<", 3) == 0)
			{
				delimiter_token = get_next_token_tokenizer(&tokenizer);
				if (!delimiter_token)
				{
					free(token);
					ft_lstclear(&tokens, free);
					return (NULL);
				}
				heredoc_fd = handle_heredoc(delimiter_token, 0, shell);
				free(delimiter_token);
				if (heredoc_fd == -1)
				{
					free(token);
					ft_lstclear(&tokens, free);
					return (NULL);
				}
				free(token);
				token = ft_strdup("<");
				ft_lstadd_back(&tokens, ft_lstnew(token));
				snprintf(fd_str, sizeof(fd_str), "/proc/self/fd/%d",
					heredoc_fd);
				ft_lstadd_back(&tokens, ft_lstnew(ft_strdup(fd_str)));
			}
			else
				ft_lstadd_back(&tokens, ft_lstnew(token));
		}
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

char	*preprocess_heredocs(char *input, t_shell *shell)
{
	char	*result;
	char	*pos;
	char	*delim_start;
	char	*delim_end;
	int		delim_len;
	char	*delimiter;
	int		heredoc_fd;
	char	replacement[256];
	int		original_len;
	int		replacement_len;
	int		result_len;
	char	*new_result;
	int		pos_offset;

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
		int quoted = (*delim_start == '\'' || *delim_start == '"');
		if (quoted)
		{
			delim_start++;
			delim_end--;
			delim_len -= 2;
		}
		delimiter = malloc(delim_len + 1);
		ft_strlcpy(delimiter, delim_start, delim_len + 1);
		delimiter[delim_len] = '\0';
		heredoc_fd = handle_heredoc(delimiter, !quoted, shell);
		free(delimiter);
		if (heredoc_fd == -1)
		{
			free(result);
			return (NULL);
		}
		//snprintf(replacement, sizeof(replacement), "</proc/self/fd/%d", heredoc_fd); // can't use snprintf !!!!!!!!
		char	*fd_str = ft_itoa(heredoc_fd);
		ft_strlcpy(replacement, "/proc/self/fd/", sizeof(replacement));
		ft_strlcat(replacement, fd_str, sizeof(replacement));
		free(fd_str);

		original_len = delim_end - pos;
		replacement_len = ft_strlen(replacement);
		result_len = ft_strlen(result);
		new_result = malloc(result_len - original_len + replacement_len + 1);
		pos_offset = pos - result;
		ft_strlcpy(new_result, result, pos_offset + 1);
		new_result[pos_offset] = '\0';
		ft_strlcat(new_result, replacement, result_len - original_len
			+ replacement_len + 1);
		ft_strlcat(new_result, delim_end, result_len - original_len
			+ replacement_len + 1);
		free(result);
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
