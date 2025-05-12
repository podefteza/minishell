/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 13:55:32 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/12 17:26:34 by carlos-j         ###   ########.fr       */
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
	free_array(commands); // Free the commands array
	return (1);
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

// grep hi <./test_files/infile


void	execute_final_command(char **args, t_shell *shell, char *final_input)
{
	// if the command has a pipe, never reaches this!!!! we need to free final_input somewhere else
	if ((args && args[0] && ft_strncmp(args[0], "exit", 5) == 0))
	{
		free(final_input);
		builtin_exit(args, shell);
		return ;
	}
	if (execute_builtin(args, shell))
	{
		free_array(args);
		free(final_input);
		return ;
	}
	//fprintf(stderr, "will execute_command\n");
	int execute_command_result = execute_command(args, shell);
	printf("result of execute_command: %d\n", execute_command_result);
	if (execute_command_result == 1)
	{
		printf("will free array of args\n");
				// print args
		for (int i = 0; args[i]; i++)
		{
			printf("args[%d]: %s\n", i, args[i]);
		}
		printf("args[2]: %s\n", args[2]);
		free_array(args); // these args we'll free are the ones allocated by split_arguments (tokenize.c:74)
	}
	fprintf(stderr, "execute_command OK, will free args in execute_final_command\n");
	// print the args to check if they are freed

	free(final_input);
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



void	handle_input(char *input, t_shell *shell)
{
	char	**args;
	char	*final_input;

	handle_signal_status(shell);
	final_input = process_initial_input(input);
	if (!final_input)
		return ;
	final_input = process_input_for_execution(final_input, shell);
	if (!final_input)
		return ;
	args = parse_command_arguments(final_input, shell);
	if (!args)
		return ;
	if (args[0] && ft_strncmp(args[0], "echo", ft_strlen("echo")) == 0)
		handle_echo_args(args);
	else
		clean_arguments(args);
	if (!validate_executable(args, shell))
	{
		//free_array(args);  // ??????????????????????????????
		free(final_input);
		return ;
	}
	execute_final_command(args, shell, final_input);
}
