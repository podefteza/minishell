/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 13:55:32 by carlos-j          #+#    #+#             */
/*   Updated: 2025/04/11 13:41:00 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_echo_command(char *cmd)
{
	return (ft_strncmp(cmd, "echo", 4) == 0);
}

int	execute_builtin(char **args, t_shell *shell)
{
	int	i;

	i = 0;
	if (!args || !args[0] || !shell)
		return (0);
	while (shell->builtins[i].cmd != NULL)
	{
		if (shell->builtins[i].cmd && ft_strncmp(args[0],
				shell->builtins[i].cmd, ft_strlen(shell->builtins[i].cmd)
				+ 1) == 0)
		{
			if (shell->builtins[i].func)
			{
				shell->exit_status = shell->builtins[i].func(args, shell);
				return (1);
			}
		}
		i++;
	}
	return (0);
}

char	*trim_spaces(const char *input)
{
	int		start;
	int		end;
	char	*trimmed;
	int		i;

	if (!input)
		return (NULL);
	start = 0;
	while (input[start] == ' ' || input[start] == '\t')
		start++;
	end = ft_strlen(input) - 1;
	while (end > start && (input[end] == ' ' || input[end] == '\t'))
		end--;
	trimmed = malloc((end - start + 2) * sizeof(char));
	if (!trimmed)
		return (NULL);
	i = 0;
	while (start <= end)
		trimmed[i++] = input[start++];
	trimmed[i] = '\0';
	return (trimmed);
}

void	handle_signal_status(t_shell *shell)
{
	if (g_signal_status)
	{
		shell->exit_status = 130;
		g_signal_status = 0;
	}
}

char	*input_with_expansion(char *final_input, t_shell *shell)
{
	char	*trimmed;

	final_input = expand_variables(final_input, shell);
	if (!final_input)
		return (ft_strdup(""));
	trimmed = ft_strtrim(final_input, " ");
	free(final_input);
	return (trimmed);
}

int	input_with_echo(char *final_input, char ***args_ptr, t_shell *shell)
{
	char	**raw_args;
	char	**echo_args;
	int		echo_len;
	int		raw_len;
	char	**final_args;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!ft_strnstr(final_input, "echo", ft_strlen(final_input)))
		return (0);
	if ((final_input[0] == '\"' || final_input[0] == '\'')
			&& final_input[ft_strlen(final_input) - 1] == final_input[0])
		return (0);
	raw_args = split_arguments(final_input);
	if (!raw_args)
		return (1);
	echo_args = handle_echo(raw_args[0], shell);
	if (!echo_args)
	{
		free_array(raw_args);
		return (1);
	}
	echo_len = 0;
	raw_len = 0;
	while (echo_args[echo_len])
		echo_len++;
	while (raw_args[raw_len])
		raw_len++;
	final_args = malloc(sizeof(char *) * (echo_len + raw_len));
	if (!final_args)
	{
		free_array(echo_args);
		free_array(raw_args);
		return (1);
	}
	i = 0;
	while (i < echo_len)
	{
		final_args[i] = ft_strdup(echo_args[i]);
		i++;
	}
	j = 1;
	while (raw_args[j])
	{
		final_args[i] = ft_strdup(raw_args[j]);
		i++;
		j++;
	}
	final_args[i] = NULL;
	free_array(echo_args);
	free_array(raw_args);
	*args_ptr = final_args;
	return (1);
}

int	input_with_pipe(char *final_input, t_shell *shell)
{
	char	**commands;

	if (!ft_strchr(final_input, '|') || !is_pipe_outside_quotes(final_input))
		return (0);
	commands = split_pipe(final_input, shell);
	if (!commands)
		return (1);
	execute_pipeline(commands, shell);
	free_array(commands);
	return (1);
}

char	*check_for_expansion(char *final_input, t_shell *shell)
{
	char	*expanded;

	if (ft_strchr(final_input, '$'))
	{
		expanded = input_with_expansion(final_input, shell);
		free(final_input);
		return (expanded);
	}
	return (final_input);
}

/*char	**clean_quotes_from_args(char **args)
{
	char	*cleaned_arg;
	int		i;

	printf("clean_quotes_from_args\n");

	if (!args)
		return (NULL);
	i = 0;
	while (args[i])
	{
		cleaned_arg = handle_quotes(args[i]);
		free(args[i]);
		args[i] = cleaned_arg;
		i++;
	}
	return (args);
}*/

void	handle_input(char *input, t_shell *shell)
{
	char	**args;
	char	*cleaned_arg;
	int		i;
	char	*final_input;

	handle_signal_status(shell);
	final_input = trim_spaces(input);
	free(input);
	if (!final_input || count_quotes(final_input))
		return ;
	final_input = check_for_expansion(final_input, shell);
	if (!final_input || input_with_pipe(final_input, shell))
	{
		free(final_input);
		return ;
	}
	if (ft_strnstr(final_input, "echo", ft_strlen(final_input))
		&& input_with_echo(final_input, &args, shell))
	{
		free(final_input);
		if (!args)
			return ;
	}
	else if (ft_strnstr(final_input, "export", ft_strlen(final_input)))
	{
		args = split_arguments(final_input);
		free(final_input);
		if (!args || !args[0])
		{
			free_array(args);
			return ;
		}
		//if (is_echo_command(args[0]))
		//	clean_quotes_from_args(args);
	}
	else
	{
		args = split_arguments(final_input);
		free(final_input);
		if (!args || !args[0])
		{
			free_array(args);
			return ;
		}
		//if (is_echo_command(args[0]))
		//	clean_quotes_from_args(args);
	}
	i = 0;
	while (args[i])
	{
		cleaned_arg = handle_quotes(args[i]);
		args[i] = cleaned_arg;
		i++;
	}
	if (execute_builtin(args, shell))
	{
		free_array(args);
		return ;
	}
	execute_command(args, shell);
	if (args)
		free_array(args);
}
