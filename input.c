/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 13:55:32 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/16 17:01:51 by carlos-j         ###   ########.fr       */
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
	if ((str[0] == '"' && str[len - 1] == '"') ||
		(str[0] == '\'' && str[len - 1] == '\''))
		return (ft_strndup(str + 1, len - 2));
	return (ft_strdup(str));
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
			cleaned = remove_surrounding_quotes(input->commands[i][j]);
			free(input->commands[i][j]);
			input->commands[i][j] = cleaned;
			j++;
		}
		i++;
	}
}



void	execute_final_command(t_shell *shell)
{
	int stdin_backup = dup(STDIN_FILENO);
    int stdout_backup = dup(STDOUT_FILENO);


	if (!shell->input.commands)
		return ;
	//printf("Executing command: %s\n", shell->input.commands[0][0]);
	if (shell->input.commands[0] && !is_pipeline(shell))
	{
		if (execute_builtins(shell, shell->input.commands[0])){
			// Restore stdio if builtin was executed
			dup2(stdin_backup, STDIN_FILENO);
			dup2(stdout_backup, STDOUT_FILENO);
			close(stdin_backup);
			close(stdout_backup);
			return;
    	}
	}
	execute_command(shell);
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
	// Input processing
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
	// Command splitting
	check_for_pipe(shell);
	split_commands(shell);
	//print commands to be executed
	/*for (int i = 0; shell->input.commands[i]; i++)
	{
		printf("Command %d: ", i);
		for (int j = 0; shell->input.commands[i][j]; j++)
		{
			printf("%s\n", shell->input.commands[i][j]);
		}
		printf("\n");
	}*/
	remove_quotes_from_commands(&shell->input);
	/*for (int i = 0; shell->input.commands[i]; i++)
	{
		printf("Command after removed quotes %d: ", i);
		for (int j = 0; shell->input.commands[i][j]; j++)
		{
			printf("%s\n", shell->input.commands[i][j]);
		}
		printf("\n");
	}*/
	// Execute all commands
	execute_final_command(shell);
	// Cleanup
	// free_commands(shell);  // You'll need to implement this
}
