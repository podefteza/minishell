/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_processing.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:12:18 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/12 22:13:36 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	do_dup(int fd, int is_output)
{
	int	dup_result;

	if (is_output)
		dup_result = dup2(fd, STDOUT_FILENO);
	else
		dup_result = dup2(fd, STDIN_FILENO);
	if (dup_result == -1)
	{
		perror("dup2 failed");
		close(fd);
		return (-1);
	}
	return (0);
}

static int	finalize_redirection(char **op, char **filename, int fd, int *count)
{
	(*count)--;
	/*printf("debug.................\n");
	printf("op: %s\n", *op);
	printf("filename: %s\n", *filename);*/
	if (*count == 0 || *count == 1)
	{
		if (!(is_redirection_operator(*op)))
		{
			//printf("debug1\n");
			free(*op); // commenting this solves issues in [< .]  [<< EOF]  [> file] also we get minishell_tester OK
			*op = NULL;
		}
		if (*filename)
		{
			//printf("debug2\n");
			//free(*filename); // commenting this solves issues in [< .]  [<< EOF]  [> file] also we get minishell_tester OK
			*filename = NULL;
		}
	}
	close(fd);
	return (0);
}

static int	redirect_command(int apply_redirection, char *op, char *filename,
		t_shell *shell, int total_redirections)
{
	int			fd;
	int			is_output;
	static int	redirection_count = 0;

	is_output = (op[0] == '>');
	fd = open_redirection_file(op, filename);
	if (fd == -1)
	{
		//printf("debug........\n");
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(filename);
		shell->exit_status = 1;
		return (-1);
	}

	// Initialize redirection_count if it's the first redirection
	if (redirection_count == 0)
		redirection_count = total_redirections;

	if (apply_redirection && do_dup(fd, is_output) == -1)
		return (-1);

	// Pass op and filename as pointers so they can be modified
	return (finalize_redirection(&op, &filename, fd, &redirection_count));
}

int	process_valid_redirection(char **args, int i, t_shell *shell,
		int total_redirections)
{
	int	apply_redirection;

	if (!args[i + 1])
	{
		ft_puterr("minishell", SNT, " `newline'", "\n");
		shell->exit_status = 2;
		return (-1);
	}
	if (is_redirection_operator(args[i + 1]))
	{
		print_redirection_syntax_error(shell, args[i + 1]);
		return (-1);
	}
	apply_redirection = 0;
	if ((i > 0) || (args[i + 2] != NULL && !is_redirection_operator(args[i
				+ 2])))
		apply_redirection = 1;
	int redirect_result = redirect_command(apply_redirection, args[i], args[i + 1], shell,
			total_redirections);
	if (redirect_result == -1)
		return (-1);
	return (0);
}

int	handle_invalid_redirection_token(char **args, int i, t_shell *shell)
{
	if (i == 0 || (i > 0 && ft_strncmp(args[i - 1], "echo", 5) != 0))
	{
		print_redirection_syntax_error(shell, args[i]);
		return (-1);
	}
	return (0);
}
