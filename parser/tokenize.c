/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:29:25 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/23 08:46:38 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	init_tokenizer(t_tokenizer *t, const char *input)
{
	t->input = input;
	t->pos = 0;
	t->done = FALSE;
	t->in_quotes = FALSE;
	t->quote_char = 0;
}

static char	**tokenize_command(const char *cmd)
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
			ft_lstadd_back(&tokens, ft_lstnew(token));
	}
	result = list_to_array(tokens);
	ft_lstclear(&tokens, free);
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

static int	fill_commands(char ***commands, char **args, int count)
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
			commands[i] = tokenize_command(args[i]);
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

	count = count_args(shell->input.args);
	shell->input.commands = malloc((count + 1) * sizeof(char **));
	if (!shell->input.commands)
		return ;
	allocated = fill_commands(shell->input.commands, shell->input.args, count);
	if (allocated == -1 || allocated < count)
	{
		int j = 0;
		while (shell->input.commands[j])
			free_array(shell->input.commands[j++]);
		free(shell->input.commands);
		shell->input.commands = NULL;
	}
	else
		shell->input.commands[count] = NULL;
}
