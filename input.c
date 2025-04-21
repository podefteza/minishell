/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 13:55:32 by carlos-j          #+#    #+#             */
/*   Updated: 2025/04/21 13:55:42 by carlos-j         ###   ########.fr       */
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

static int	is_empty_quoted_string(const char *str)
{
	return (str[0] == '\"' && str[1] == '\"' && str[2] == '\0');
}

// for loops...
int	input_with_echo(char *final_input, char ***args_ptr, t_shell *shell)
{
	char	**raw_args;
	char	**echo_args;
	int		echo_len;
	int		raw_len;
	char	**final_args;
	char	**temp_args;
	int		new_raw_len;
	int		has_echo;
	char	*trimmed;
	char	*cmd;

	if ((final_input[0] == '\"' || final_input[0] == '\'') && final_input[1] == final_input[0])
		final_input += 2;

	int i, j, z;
	has_echo = 0;
	if (!ft_strnstr(final_input, "echo", ft_strlen(final_input)))
		return (0);
	trimmed = trim_spaces(final_input);
	if (!trimmed)
		return (0);
	if ((trimmed[0] == '"' || trimmed[0] == '\'') && ft_strnstr(trimmed + 1,
			"echo", ft_strlen(trimmed) - 1) == trimmed + 1)
	{
		cmd = malloc(ft_strlen(trimmed) - 1);
		if (!cmd)
		{
			free(trimmed);
			return (0);
		}
		ft_strlcpy(cmd, trimmed + 1, ft_strlen(trimmed) - 1);
		raw_args = split_arguments(cmd);
		free(cmd);
		free(trimmed);
		if (!raw_args || !raw_args[0])
		{
			if (raw_args)
				free_array(raw_args);
			return (0);
		}
		*args_ptr = raw_args;
		return (1);
	}
	free(trimmed);
	new_raw_len = 0;
	raw_args = split_arguments(final_input);
	if (!raw_args)
		return (1);
	if (ft_strncmp(raw_args[0], "echo", 5) == 0 && raw_args[0][4] == '\0')
		has_echo = 1;

	if (!has_echo)
	{
		free_array(raw_args);
		return (0);
	}
	z = 0;
	while (raw_args[z])
	{
		if (!is_empty_quoted_string(raw_args[z]))
			new_raw_len++;
		z++;
	}
	temp_args = malloc(sizeof(char *) * (new_raw_len + 1));
	if (!temp_args)
	{
		free_array(raw_args);
		return (1);
	}
	z = 0;
	j = 0;
	while (raw_args[z])
	{
		if (!is_empty_quoted_string(raw_args[z]))
		{
			temp_args[j] = ft_strdup(raw_args[z]);
			j++;
		}
		z++;
	}
	temp_args[j] = NULL;
	free_array(raw_args);
	raw_args = temp_args;
	echo_args = handle_echo(raw_args[0], shell);
	if (!echo_args)
	{
		free_array(raw_args);
		return (1);
	}
	echo_len = 0;
	while (echo_args[echo_len])
		echo_len++;
	raw_len = 0;
	while (raw_args[raw_len])
		raw_len++;
	final_args = malloc(sizeof(char *) * (echo_len + raw_len));
	if (!final_args)
	{
		free_array(echo_args);
		free_array(raw_args);
		return (1);
	}
	for (i = 0; i < echo_len; i++)
		final_args[i] = ft_strdup(echo_args[i]);
	for (j = 1; j < raw_len; j++, i++)
		final_args[i] = ft_strdup(raw_args[j]);
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
	char *tilde_expanded = expand_tilde_unquoted(final_input);
	free(final_input);

	final_input = tilde_expanded;
	if (validate_syntax(final_input, shell))
	{
		free(final_input);
		return ;
	}
	final_input = check_for_expansion(final_input, shell);
	if (!final_input || input_with_pipe(final_input, shell))
	{
		free(final_input);
		return ;
	}
	else if (ft_strnstr(final_input, "echo", ft_strlen(final_input))
		&& input_with_echo(final_input, &args, shell) && !is_quoted(final_input))
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
	}
	i = 0;
	while (args[i])
	{
		cleaned_arg = handle_quotes(args[i]);
		args[i] = cleaned_arg;
		i++;
	}
	if (args[0] && args[0][0] == '/')
	{
		if (access(args[0], F_OK) != 0)
		{
			ft_puterr("minishell: ", args[0], ": No such file or directory\n", "");
			free_array(args);
			shell->exit_status = 127;
			return ;
		}
	}
	if (execute_builtin(args, shell))
	{
		if (args)
			free_array(args);
		return ;
	}
	execute_command(args, shell);
	if (args)
		free_array(args);
}
