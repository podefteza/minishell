/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:04:57 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/08 13:12:10 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*process_initial_input(char *input)
{
	char	*final_input;
	char	*tmp;

	final_input = trim_spaces(input);
	free(input);
	if (!final_input || count_quotes(final_input))
		return (NULL);
	tmp = expand_tilde_unquoted(final_input);
	free(final_input);
	return (tmp);
}

char	*interpret_escapes(const char *input)
{
	char	*result;
	int		i;
	int		j;
	int		in_single;
	int		in_double;

	result = malloc(ft_strlen(input) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	in_single = 0;
	in_double = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (input[i] == '"' && !in_single)
			in_double = !in_double;
		else if (input[i] == '\\' && input[i + 1] && !in_single)
		{
			if (input[i + 1] == '\\' || input[i + 1] == '"'
				|| input[i + 1] == '\'' || input[i + 1] == '$')
				result[j++] = input[i++];
			else
				i++;
		}
		result[j++] = input[i++];
	}
	result[j] = '\0';
	return (result);
}

char	*process_input_for_execution(char *input, t_shell *shell)
{
	if (validate_syntax(input, shell))
	{
		free(input);
		return (NULL);
	}
	input = interpret_escapes(input);
	if (!input)
		return (NULL);
	input = check_for_expansion(input, shell);
	if (!input || input_with_pipe(input, shell))
	{
		free(input);
		return (NULL);
	}
	return (input);
}

char	**handle_echo_or_export(char *input, t_shell *shell)
{
	char	**args;

	if (ft_strnstr(input, "echo", ft_strlen(input)) && input_with_echo(input,
			&args, shell) && !is_quoted(input))
	{
		free(input);
		if (!args)
			return (NULL);
		return (args);
	}
	if (ft_strnstr(input, "export", ft_strlen(input)))
	{
		args = split_arguments(input);
		free(input);
		if (!args || !args[0])
		{
			free_array(args);
			return (NULL);
		}
		return (args);
	}
	return (NULL);
}

char	**parse_command_arguments(char *input, t_shell *shell)
{
	char	**args;

	args = handle_echo_or_export(input, shell);
	if (args)
		return (args);
	(void)shell;
	args = split_arguments(input);
	free(input);
	if (!args || !args[0])
	{
		free_array(args);
		return (NULL);
	}
	return (args);
}

void	clean_arguments(char **args)
{
	int		i;
	char	*cleaned_arg;

	i = 0;
	while (args[i])
	{
		cleaned_arg = handle_quotes(args[i]);
		args[i] = cleaned_arg;
		i++;
	}
}
