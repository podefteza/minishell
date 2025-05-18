/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:29:25 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/16 16:26:21 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**list_to_array(t_list *lst)
{
	int		len;
	char	**arr;
	int		i;

	len = ft_lstsize(lst);
	arr = malloc((len + 1) * sizeof(char *));
	if (!arr)
		return (NULL);
	i = 0;
	while (lst)
	{
		arr[i++] = lst->content;
		lst = lst->next;
	}
	arr[i] = NULL;
	return (arr);
}

void	free_2d_array(char **array)
{
	if (!array)
		return ;
	for (int i = 0; array[i]; i++)
		free(array[i]);
	free(array);
}

char	*ft_strndup(const char *s, size_t n)
{
	char	*dup;
	size_t	len;

	len = ft_strlen(s);
	if (n < len)
		len = n;
	dup = malloc(len + 1);
	if (!dup)
		return (NULL);
	ft_memcpy(dup, s, len);
	dup[len] = '\0';
	return (dup);
}

void	init_tokenizer(t_tokenizer *t, const char *input)
{
	t->input = input;
	t->pos = 0;
	t->done = false;
	t->in_quotes = false;
	t->quote_char = 0;
}

char	*get_quoted_token(t_tokenizer *t)
{
	int	start;

	start = t->pos;
	while (t->input[t->pos] && t->input[t->pos] != t->quote_char)
		t->pos++;
	if (t->input[t->pos] != t->quote_char)
	{
		return (NULL);
	}
	return (ft_strndup(t->input + start, t->pos - start));
}

char	*get_redirection_token(t_tokenizer *t)
{
	char	redir;
	char	*token;

	redir = t->input[t->pos++];
	if (t->input[t->pos] == redir)
	{
		token = malloc(3);
		token[0] = redir;
		token[1] = redir;
		token[2] = '\0';
		t->pos++;
		return (token);
	}
	token = malloc(2);
	token[0] = redir;
	token[1] = '\0';
	return (token);
}

char	*get_normal_token(t_tokenizer *t)
{
	int	start;

	start = t->pos;
	while (t->input[t->pos] && !isspace(t->input[t->pos]))
	{
		if (!t->in_quotes && strchr("><|", t->input[t->pos]))
			break ;
		t->pos++;
	}
	return (ft_strndup(t->input + start, t->pos - start));
}

char	*get_next_token_tokenizer(t_tokenizer *t)
{
	int		start;
	bool	escaped;

	escaped = false;
	while (isspace(t->input[t->pos]))
		t->pos++;
	if (t->input[t->pos] == '\0')
	{
		t->done = true;
		return (NULL);
	}

	start = t->pos;
	while (t->input[t->pos])
	{
		if (!t->in_quotes && (t->input[t->pos] == ' ' || t->input[t->pos] == '\t'))
			break ;
		if (!t->in_quotes && strchr("><|", t->input[t->pos]))
			break ;
		if (!escaped && (t->input[t->pos] == '\'' || t->input[t->pos] == '\"'))
		{
			if (!t->in_quotes)
			{
				t->in_quotes = true;
				t->quote_char = t->input[t->pos];
			}
			else if (t->quote_char == t->input[t->pos])
			{
				t->in_quotes = false;
			}
		}
		t->pos++;
		if (!t->in_quotes && (isspace(t->input[t->pos]) || strchr("><|", t->input[t->pos])))
			break ;
	}

	if (t->pos == start)
	{
		// Special token (like >, >>, |), parse it separately
		return (get_redirection_token(t));
	}
	return (ft_strndup(t->input + start, t->pos - start));
}


char	**tokenize_command(const char *cmd)
{
	t_tokenizer	tokenizer;
	t_list		*tokens;
	char		*token;
	char		**result;

	init_tokenizer(&tokenizer, cmd);
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
			ft_lstadd_back(&tokens, ft_lstnew(token));
		}
	}
	result = list_to_array(tokens);
	ft_lstclear(&tokens, NULL);
	return (result);
}

void	split_commands(t_shell *shell)
{
	int	cmd_count;

	cmd_count = 0;
	while (shell->input.args[cmd_count])
		cmd_count++;
	shell->input.commands = malloc((cmd_count + 1) * sizeof(char **));
	if (!shell->input.commands)
		return ;
	for (int i = 0; shell->input.args[i]; i++)
	{
		if (ft_strncmp(shell->input.args[i], "|", 2) == 0)
		{
			shell->input.commands[i] = malloc(2 * sizeof(char *));
			shell->input.commands[i][0] = ft_strdup("|");
			shell->input.commands[i][1] = NULL;
			continue ;
		}
		shell->input.commands[i] = tokenize_command(shell->input.args[i]);
		if (!shell->input.commands[i])
		{
			while (i-- > 0)
				free_2d_array(shell->input.commands[i]);
			free(shell->input.commands);
			shell->input.commands = NULL;
			return ;
		}
	}
	shell->input.commands[cmd_count] = NULL;
}
