/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 13:55:32 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/13 09:47:25 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	input_with_pipe(char *final_input, t_shell *shell)
{
	char	**commands;

	if (!ft_strchr(final_input, '|') || !is_pipe_outside_quotes(final_input))
		return (0);
	commands = split_pipe(final_input, shell);
	if (!commands)
	{
		free(final_input); // remove this if we get errors
		return (1);
	}
	free(final_input);
	execute_pipeline(commands, shell);
	//printf("we'll get here\n");
	free_array(commands); // Free the commands array
	return (1);
}

int	validate_executable(char **args, t_shell *shell)
{
	if (args[0] && args[0][0] == '/')
	{
		if (access(args[0], F_OK) != 0)
		{
			//printf("minishell: %s: %s%s", args[0], NFD, "\n");
			ft_puterr("minishell: ", args[0], ": No such file or directory\n",
				"");
			free_array(args);
			shell->exit_status = 127;
			return (0);
		}
	}
	return (1);
}

// grep hi <./test_files/infile


void	execute_final_command(t_shell *shell)
{
	// if the command has a pipe, never reaches this!!!! we need to free final_input somewhere else
	if ((shell->input.args && shell->input.args[0] && ft_strncmp(shell->input.args[0], "exit", 5) == 0))
	{
		free(shell->input.processed);
		builtin_exit(shell->input.args, shell);
		return ;
	}
	if (execute_builtin(shell->input.args, shell))
	{
		//printf("shell->input.raw: %s\n", shell->input.raw);
		free_array(shell->input.args);
		free(shell->input.processed);
		return ;
	}
	//fprintf(stderr, "will execute_command\n");
	int execute_command_result = execute_command(shell->input.args, shell);
	//printf("result of execute_command: %d\n", execute_command_result);
	if (execute_command_result == 1)
	{
		//printf("will free array of args\n");
				// print args
		/*for (int i = 0; args[i]; i++)
		{
			printf("args[%d]: %s\n", i, args[i]);
		}*/
		//printf("args[2]: %s\n", args[2]);
		free_array(shell->input.args); // these args we'll free are the ones allocated by split_arguments (tokenize.c:74)
	}
	//fprintf(stderr, "execute_command OK, will free args in execute_final_command\n");
	// print the args to check if they are freed


	free(shell->input.processed);
}

void	handle_echo_args(char **args) // check if this is being used, remove it!!!!!!!!!!!!!
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
	shell->input.processed = process_input_for_execution(shell);
	if (!shell->input.processed)
		return ;
	parse_command_arguments(shell);
	if (!shell->input.args)
		return ;
	if (shell->input.args[0] && ft_strncmp(shell->input.args[0], "echo", ft_strlen("echo")) == 0)
		handle_echo_args(shell->input.args);
	else
		clean_arguments(shell);
	if (!validate_executable(shell->input.args, shell))
	{
		free(shell->input.processed);
		return ;
	}
	execute_final_command(shell);
}
