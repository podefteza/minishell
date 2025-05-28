/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:04:01 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/28 01:29:39 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	g_signal_status = 1;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_done = 1;
	rl_redisplay();
}

// version with rl_cleanup_after_signal and rl_reset_terminal
/*int	handle_heredoc(char *delimiter, int expand, t_shell *shell)
{
	int					fd[2];
	char				*line;
	struct termios		original_term;
	struct sigaction	sa_int;
	struct sigaction	sa_quit;
	struct sigaction	old_sa_int;
	struct sigaction	old_sa_quit;

	if (pipe(fd) == -1)
		return (-1);
	tcgetattr(STDIN_FILENO, &original_term);
	sigaction(SIGINT, NULL, &old_sa_int);
	sigaction(SIGQUIT, NULL, &old_sa_quit);
	sa_int.sa_handler = heredoc_sigint_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
	while (!g_signal_status)
	{
		line = readline("> ");
		if (!line)
			break;
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
		{
			free(line);
			break;
		}
		if (expand)
			line = expand_variables(line, shell);
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	close(fd[1]);
	if (g_signal_status)
	{
		close(fd[0]);
		tcsetattr(STDIN_FILENO, TCSANOW, &original_term);
		rl_reset_terminal(NULL);
		rl_cleanup_after_signal();
		sigaction(SIGINT, &old_sa_int, NULL);
		sigaction(SIGQUIT, &old_sa_quit, NULL);
		int i = 3;
		while (i < 1024)
			close(i++);
		return (-1);
	}
	sigaction(SIGINT, &old_sa_int, NULL);
	sigaction(SIGQUIT, &old_sa_quit, NULL);
	return (fd[0]);
}*/

int	handle_heredoc(char *delimiter, int expand, t_shell *shell)
{
	int					fd[2];
	char				*line;
	struct termios		original_term;
	struct sigaction	sa_int;
	struct sigaction	sa_quit;
	struct sigaction	old_sa_int;
	struct sigaction	old_sa_quit;

	if (pipe(fd) == -1)
		return (-1);
	tcgetattr(STDIN_FILENO, &original_term);
	sigaction(SIGINT, NULL, &old_sa_int);
	sigaction(SIGQUIT, NULL, &old_sa_quit);
	sa_int.sa_handler = heredoc_sigint_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
	while (!g_signal_status)
	{
		line = readline("> ");
		if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
		{
			free(line);
			break;
		}
		if (expand)
		{
			char *expanded = expand_variables(line, shell);
			free(line);
			line = expanded;
		}
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	close(fd[1]);
	tcsetattr(STDIN_FILENO, TCSANOW, &original_term);
	sigaction(SIGINT, &old_sa_int, NULL);
	sigaction(SIGQUIT, &old_sa_quit, NULL);
	if (g_signal_status)
	{
		close(fd[0]);
		return (-1);
	}
	return (fd[0]);
}


static int	apply_redirection(char *op, int fd, int *stored_fd)
{
	int	target_fd;

	if (*stored_fd != -1)
		close(*stored_fd);
	*stored_fd = fd;
	if (op[0] == '<')
		target_fd = STDIN_FILENO;
	else
		target_fd = STDOUT_FILENO;
	if (dup2(fd, target_fd) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	return (0);
}

static int	process_redirection(char *op, char *filename, t_shell *shell,
		int *fd_pair)
{
	int	fd;

	if (!filename)
	{
		ft_puterr("minishell", SNT, " `newline'", "\n");
		shell->exit_status = 2;
		return (-1);
	}
	fd = open_redirection_file(op, filename);
	if (fd == -1)
	{
		if (!g_signal_status)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			perror(filename);
			shell->exit_status = 1;
			return (-1);
		}
		else
		{
			//g_signal_status = 0; // Reset signal status after handling error
			return (-1);
		}
	}
	if (op[0] == '<')
	{
		if (apply_redirection(op, fd, &fd_pair[0]) == -1)
			return (-1);
	}
	else if (apply_redirection(op, fd, &fd_pair[1]) == -1)
		return (-1);
	return (0);
}

static int	compact_args_array(char **args)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (args[i])
	{
		if (is_redirection_operator(args[i]) && args[i + 1])
		{
			free(args[i]);
			free(args[i + 1]);
			i += 2;
		}
		else
			args[j++] = args[i++];
	}
	args[j] = NULL;
	return (j);
}

int	handle_redirections(char **args, t_shell *shell)
{
	int	i;
	int	fd_pair[2];
	int	result;

	i = 0;
	fd_pair[0] = -1;
	fd_pair[1] = -1;
	while (args[i])
	{
		if (is_redirection_operator(args[i]))
		{
			result = process_redirection(args[i], args[i + 1], shell, fd_pair);
			if (result == -1)
				return (-1);
			i += 2;
		}
		else
			i++;
	}
	compact_args_array(args);
	if (fd_pair[0] != -1)
		close(fd_pair[0]);
	if (fd_pair[1] != -1)
		close(fd_pair[1]);
	return (0);
}
