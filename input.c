/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 13:55:32 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/20 14:16:18 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_for_pipe(t_shell *shell)
{
	if (!ft_strchr(shell->input.processed, '|')
		|| !is_pipe_outside_quotes(shell->input.processed))
	{
		shell->input.args = malloc(2 * sizeof(char *));
		if (!shell->input.args)
			return ;
		shell->input.args[0] = shell->input.processed;
		shell->input.args[1] = NULL;
		return ;
	}
	shell->input.args = split_pipe(shell);
	if (!shell->input.args)
	{
		shell->input.args = malloc(2 * sizeof(char *));
		if (!shell->input.args)
			return ;
		shell->input.args[0] = shell->input.processed;
		shell->input.args[1] = NULL;
	}
}

int	validate_executable(char **args, t_shell *shell)
{
	if (args[0] && args[0][0] == '/')
	{
		if (access(args[0], F_OK) != 0)
		{
			ft_puterr("minishell: ", args[0], ": No such file or directory\n",
				"");
			free_array(args);
			shell->exit_status = 127;
			return (0);
		}
	}
	return (1);
}

int	is_pipeline(t_shell *shell)
{
	if (!shell->input.commands)
		return (0);
	for (int i = 0; shell->input.commands[i]; i++)
	{
		if (shell->input.commands[i][0]
			&& ft_strncmp(shell->input.commands[i][0], "|", 2) == 0)
		{
			return (1);
		}
	}
	return (0);
}

char	*remove_surrounding_quotes(const char *str)
{
	size_t	len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if ((str[0] == '"' && str[len - 1] == '"') || (str[0] == '\'' && str[len
			- 1] == '\''))
		return (ft_strndup(str + 1, len - 2));
	return (ft_strdup(str));
}

char	*remove_quotes_concat(const char *str)
{
	int		i;
	int		j;
	char	quote;
	char	*result;

	i = 0;
	j = 0;
	quote = 0;
	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	while (str[i])
	{
		if ((str[i] == '"' || str[i] == '\'') && !quote)
		{
			quote = str[i++];
			continue ;
		}
		else if (str[i] == quote)
		{
			quote = 0;
			i++;
			continue ;
		}
		result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

void	remove_quotes_from_commands(t_input *input)
{
	int		i;
	int		j;
	char	*cleaned;

	i = 0;
	while (input->commands && input->commands[i])
	{
		j = 0;
		while (input->commands[i][j])
		{
			cleaned = remove_quotes_concat(input->commands[i][j]);
			free(input->commands[i][j]);
			input->commands[i][j] = cleaned;
			j++;
		}
		i++;
	}
}

void	execute_final_command(t_shell *shell)
{
	int	stdin_backup;
	int	stdout_backup;

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);

	/*if ((shell->input.args && shell->input.args[0]
			&& ft_strncmp(shell->input.args[0], "exit", 5) == 0))
	{
		builtin_exit(shell->input.args, shell);
		return ;
	}*/
	if (!shell->input.commands)
		return ;

	if (shell->input.commands[0] && !is_pipeline(shell))
	{
		if (execute_builtins(shell, shell->input.commands[0]))
		{
			dup2(stdin_backup, STDIN_FILENO);
			dup2(stdout_backup, STDOUT_FILENO);
			close(stdin_backup);
			close(stdout_backup);
			if (shell->should_exit == 1)
				exit(shell->exit_status);
			return ;
		}
	}
	execute_command(shell);
	printf("after execute_command\n");
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);
}

void	handle_echo_args(char **args)
{
	int		i;
	int		len;
	int		is_quoted;
	char	*cleaned;

	i = 1;
	while (args[i])
	{
		len = ft_strlen(args[i]);
		is_quoted = (len >= 2 && ((args[i][0] == '"' && args[i][len - 1] == '"')
					|| (args[i][0] == '\'' && args[i][len - 1] == '\'')));
		if (i != 1 && is_quoted)
		{
			cleaned = handle_quotes(ft_strdup(args[i]));
			if (cleaned)
			{
				free(args[i]);
				args[i] = cleaned;
			}
		}
		i++;
	}
}


void	handle_input(t_shell *shell)
{
	handle_signal_status(shell);
	shell->input.processed = process_initial_input(shell->input.raw);
	if (!shell->input.processed)
		return ;
	shell->input.processed = check_for_expansion(shell);
	if (!shell->input.processed || shell->input.processed[0] == '\0')
	{
		free(shell->input.processed);
		return ;
	}
	if (validate_syntax(shell))
	{
		free(shell->input.processed);
		return ;
	}
	check_for_pipe(shell);
	split_commands(shell);
	remove_quotes_from_commands(&shell->input);
	execute_final_command(shell);
	// free shell->input.commands
	int cmd_idx = 0;
	while (shell->input.commands[cmd_idx])
	{
		free(shell->input.commands[cmd_idx]);
		cmd_idx++;
	}
}
