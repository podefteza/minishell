/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_with_echo.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 13:36:54 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/13 11:18:16 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_wrapped_echo(char *trimmed, char ***args_ptr)
{
	char	**raw_args;
	char	*cmd;

	cmd = malloc(ft_strlen(trimmed) - 1);
	if (!cmd)
		return (0);
	ft_strlcpy(cmd, trimmed + 1, ft_strlen(trimmed) - 1);
	raw_args = split_arguments(cmd);
	free(cmd);
	if (!raw_args || !raw_args[0])
	{
		if (raw_args)
			free_array(raw_args);
		return (0);
	}
	*args_ptr = raw_args;
	return (1);
}

int	finalize_echo_args(t_shell *shell)
{
	char	**raw_args;
	char	**temp_args;
	char	**echo_args;
	int		len;

	raw_args = split_arguments(shell->input.processed);
	if (!raw_args)
		return (1);
	if (ft_strncmp(raw_args[0], "echo", 5) != 0 || raw_args[0][4] != '\0')
	{
		free_array(raw_args);
		return (0);
	}
	temp_args = duplicate_non_empty_args(raw_args, &len);
	free_array(raw_args);
	if (!temp_args)
		return (1);

	echo_args = handle_echo(temp_args[0], shell);

	// print echo args
	/*for (int i = 0; echo_args[i]; i++)
	{
		printf("echo_args[%d]: %s\n", i, echo_args[i]);
	}*/


	if (!echo_args || merge_echo_and_args(&shell->input.args, temp_args, echo_args))
	{
		free_array(echo_args);
		free_array(temp_args);
		return(1);
	}

	//printf("we're here...\n");
	free_array(echo_args);
	free_array(temp_args);
	return (1);  // Return 1 to indicate echo command was processed
}

int	input_with_echo(t_shell *shell)
{
	char	*trimmed;
	int     result;

	if ((shell->input.processed[0] == '\"' || shell->input.processed[0] == '\'')
		&& shell->input.processed[1] == shell->input.processed[0])
		shell->input.processed += 2;
	if (!ft_strnstr(shell->input.processed, "echo", ft_strlen(shell->input.processed)))
		return (0);
	trimmed = trim_spaces(shell->input.processed);
	if (!trimmed)
		return (0);
	if ((trimmed[0] == '"' || trimmed[0] == '\'') && ft_strnstr(trimmed + 1,
			"echo", ft_strlen(trimmed) - 1) == trimmed + 1)
	{
		result = handle_wrapped_echo(trimmed, &shell->input.args);
		free(trimmed);
		return (result);
	}
	free(trimmed);
	result = finalize_echo_args(shell);
	if (result == 1 && shell->input.args == NULL)
	{
		free_array(shell->input.args);
		shell->input.args = NULL;
	}

	// print final args
	for (int i = 0; shell->input.args && shell->input.args[i]; i++)
	{
		//printf("final_args[%d]: %s\n", i, shell->input.args[i]);
	}



	// will return 1
	return (result);
}
