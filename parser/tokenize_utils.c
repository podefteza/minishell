/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 17:11:38 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/21 21:06:55 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
		arr[i] = ft_strdup(lst->content);
		if (!arr[i])
		{
			while (i-- > 0)
				free(arr[i]);
			free(arr);
			return (NULL);
		}
		i++;
		lst = lst->next;
	}
	arr[i] = NULL;
	return (arr);
}

static char	*get_redirection_token(t_tokenizer *t)
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

static void	skip_token_while_valid(t_tokenizer *t)
{
	while (t->input[t->pos])
	{
		if (!t->in_quotes && (t->input[t->pos] == ' '
				|| t->input[t->pos] == '\t'))
			break ;
		if (!t->in_quotes && strchr("><|", t->input[t->pos]))
			break ;
		if (t->input[t->pos] == '\'' || t->input[t->pos] == '\"')
		{
			if (!t->in_quotes)
			{
				t->in_quotes = TRUE;
				t->quote_char = t->input[t->pos];
			}
			else if (t->quote_char == t->input[t->pos])
				t->in_quotes = FALSE;
		}
		t->pos++;
		if (!t->in_quotes && (isspace(t->input[t->pos]) || strchr("><|",
					t->input[t->pos])))
			break ;
	}
}

char	*get_next_token_tokenizer(t_tokenizer *t)
{
	int	start;

	while (isspace(t->input[t->pos]))
		t->pos++;
	if (t->input[t->pos] == '\0')
	{
		t->done = TRUE;
		return (NULL);
	}
	start = t->pos;
	skip_token_while_valid(t);
	if (t->pos == start)
		return (get_redirection_token(t));
	return (ft_strndup(t->input + start, t->pos - start));
}
