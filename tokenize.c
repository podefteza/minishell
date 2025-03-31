/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:29:25 by carlos-j          #+#    #+#             */
/*   Updated: 2025/03/31 09:55:13 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int is_redirection(char c)
{
    return (c == '<' || c == '>');
}

static int is_quoted(char *str)
{
    if (!str || ft_strlen(str) < 2)
        return 0;
    return ((str[0] == '\'' && str[ft_strlen(str)-1] == '\'') ||
           (str[0] == '"' && str[ft_strlen(str)-1] == '"'));
}

char *get_next_token(char *input)
{
    static char *str;
    char *token;
    int in_squote = 0;
    int in_dquote = 0;
    int i = 0;
    int token_started = 0;

    if (input)
        str = input;
    if (!str || *str == '\0')
        return NULL;
    while (*str && (*str == ' ' || *str == '\t'))
        str++;
    if (!in_squote && !in_dquote && is_redirection(*str))
    {
        token = malloc(3);
        if (!token)
            return NULL;
        token[i++] = *str++;
        if ((*str == '>' && token[0] == '>') || (*str == '<' && token[0] == '<'))
            token[i++] = *str++;
        token[i] = '\0';
        return token;
    }
    token = malloc(ft_strlen(str) + 1);
    if (!token)
        return NULL;

    while (*str)
    {
        if (*str == '\'' && !in_dquote)
            in_squote = !in_squote;
        else if (*str == '\"' && !in_squote)
            in_dquote = !in_dquote;
        else if (*str == ' ' && !in_squote && !in_dquote && token_started)
            break;
        if (!in_squote && !in_dquote && is_redirection(*str) && token_started)
            break;
        if (*str != ' ' || in_squote || in_dquote)
        {
            token[i++] = *str;
            token_started = 1;
        }
        str++;
    }
    token[i] = '\0';
    while (*str == ' ' || *str == '\t')
        str++;
    if (!*str)
        str = NULL;

    return token;
}

char **split_arguments(char *input)
{
    int arg_count;
    char **args;
    int i;
    char *token;
	int redirection_count;

    if (!input)
        return (NULL);
    arg_count = count_words(input);
	redirection_count = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '<' || input[i] == '>')
		{
			redirection_count++;
			if (input[i + 1] == '<' || input[i + 1] == '>')
				i++;
		}
		i++;
	}
    args = malloc(sizeof(char *) * (arg_count + redirection_count + 1));
    if (!args)
        return (NULL);
    i = 0;
    token = get_next_token(input);
	if (!token)
	{
		args[0] = ft_strdup(input);  // Ensure args[0] gets a valid allocated copy
		if (!args[0])
		{
			free(args);  // Free args before returning NULL to prevent memory leaks
			return (NULL);
		}
		args[1] = NULL;
		return (args);
	}


    while (token)
    {
        if (!is_quoted(token) &&
            (ft_strncmp(token, "<", 2) == 0 ||
            ft_strncmp(token, ">", 2) == 0 ||
            ft_strncmp(token, ">>", 3) == 0 ||
            ft_strncmp(token, "<<", 3) == 0))
        {
            args[i++] = ft_strdup(token);
            free(token);
            token = get_next_token(NULL);
            if (token)
            {
                args[i++] = ft_strdup(token);
                free(token);
            }
            else
            {
                ft_putstr_fd("Error: missing filename after redirection operator\n", 2);
				// update exit status...
                return (NULL);
            }
        }
        else
        {
            args[i++] = ft_strdup(token);
            free(token);
        }
        token = get_next_token(NULL);
    }
    args[i] = NULL;
    return (args);
}
