/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_processing.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:12:18 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/12 16:30:46 by carlos-j         ###   ########.fr       */
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
	// print operation and filename for debugging
	//fprintf(stderr, "op = %s\n", *op);
	//fprintf(stderr, "filename = %s\n", *filename);
	//fprintf(stderr, "fd = %d\n", fd);


	// Decrement the count
	(*count)--;
	//fprintf(stderr, "count = %d\n", *count);

	// Only free op and filename when count reaches 0





	if (*count == 0 || *count == 1)
	{
		// Safely free op if it exists
		if (*op)
		{
			//free(*op); // commenting this solves issues in [< .]  [<< EOF]  [> file] also we get minishell_tester OK
			*op = NULL;
		}

		// Safely free filename if it exists
		if (*filename)
		{
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
