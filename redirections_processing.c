/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_processing.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:12:18 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/16 15:02:26 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int redirect_command(int apply_redirection, char *op, char *filename,
                          t_shell *shell, int total_redirections)
{
    (void)total_redirections; // Unused variable
	int fd = open_redirection_file(op, filename);
    if (fd == -1) {
        ft_putstr_fd("minishell: ", STDERR_FILENO);
        perror(filename);
        shell->exit_status = 1;
        return -1;
    }

    if (apply_redirection) {
        int target_fd = (op[0] == '>') ? STDOUT_FILENO : STDIN_FILENO;
        if (dup2(fd, target_fd) == -1) {
            perror("dup2 failed");
            close(fd);
            return -1;
        }
    }

    close(fd);  // Close the file descriptor after dup2
    return 0;
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
