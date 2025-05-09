/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_with_echo.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 13:36:54 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/09 16:26:25 by carlos-j         ###   ########.fr       */
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

int	finalize_echo_args(char *input, char ***args_ptr, t_shell *shell)
{
	char	**raw_args;
	char	**temp_args;
	char	**echo_args;
	int		len;

	raw_args = split_arguments(input);
	if (!raw_args)
		return (1);
	if (ft_strncmp(raw_args[0], "echo", 5) != 0 || raw_args[0][4] != '\0')
		return (free_array(raw_args), 0);
	temp_args = duplicate_non_empty_args(raw_args, &len);
	free_array(raw_args);
	if (!temp_args)
		return (1);
	echo_args = handle_echo(temp_args[0], shell);
	if (!echo_args || merge_echo_and_args(args_ptr, temp_args, echo_args))
		return (free_array(echo_args), free_array(temp_args), 1);
	free_array(echo_args);
	free_array(temp_args);
	return (1);
}

int	input_with_echo(char *final_input, char ***args_ptr, t_shell *shell)
{
	char	*trimmed;

	if ((final_input[0] == '\"' || final_input[0] == '\'')
		&& final_input[1] == final_input[0])
		final_input += 2;
	if (!ft_strnstr(final_input, "echo", ft_strlen(final_input)))
		return (0);
	trimmed = trim_spaces(final_input);
	if (!trimmed)
		return (0);
	if ((trimmed[0] == '"' || trimmed[0] == '\'') && ft_strnstr(trimmed + 1,
			"echo", ft_strlen(trimmed) - 1) == trimmed + 1)
	{
		if (handle_wrapped_echo(trimmed, args_ptr))
		{
			free(trimmed);
			return (1);
		}
		free(trimmed);
		return (0);
	}
	free(trimmed);
	return (finalize_echo_args(final_input, args_ptr, shell));
}
