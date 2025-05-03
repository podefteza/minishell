/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 14:15:03 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/03 17:45:42 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	io_backup(int *stdin_backup, int *stdout_backup)
{
	*stdin_backup = dup(STDIN_FILENO);
	*stdout_backup = dup(STDOUT_FILENO);
	if (*stdin_backup == -1 || *stdout_backup == -1)
		return (-1);
	return (0);
}

static void	restore_io(int stdin_backup, int stdout_backup)
{
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);
}

static int	handle_background(char **args, int arg_count)
{
	if (arg_count > 0 && ft_strncmp(args[arg_count - 1], "&", 2) == 0)
	{
		free(args[arg_count - 1]);
		args[arg_count - 1] = NULL;
		return (1);
	}
	return (0);
}

static char	*validate_and_find_command(char **args, t_shell *shell)
{
	char		*full_path;
	struct stat	st;

	if (!args[0] || args[0][0] == '\0')
		return (NULL);
	full_path = find_command(args[0], shell);
	if (!full_path)
		return (NULL);
	if (stat(full_path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		is_directory(full_path, shell);
		free(full_path);
		return (NULL);
	}
	return (full_path);
}

void	execute_command(char **args, t_shell *shell)
{
	char	*full_path;
	int		stdin_backup;
	int		stdout_backup;
	int		arg_count;
	int		is_background;

	if (!args || !args[0] || io_backup(&stdin_backup, &stdout_backup) == -1)
		return ;
	if (handle_redirections(args, shell) == -1)
	{
		restore_io(stdin_backup, stdout_backup);
		return ;
	}
	arg_count = 0;
	while (args[arg_count])
		arg_count++;
	is_background = handle_background(args, arg_count);
	full_path = validate_and_find_command(args, shell);
	if (full_path)
	{
		execute_process(full_path, args, is_background, shell);
		if (full_path != args[0])
			free(full_path);
	}
	restore_io(stdin_backup, stdout_backup);
}
