/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:32:02 by carlos-j          #+#    #+#             */
/*   Updated: 2025/04/29 11:53:07 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_commands(char **commands)
{
	int	count;

	count = 0;
	while (commands[count])
		count++;
	return (count);
}

char	**copy_args(char **args)
{
	int		count;
	char	**copy;
	int		i;
	int		j;

	count = 0;
	while (args[count])
		count++;
	copy = malloc(sizeof(char *) * (count + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < count)
	{
		copy[i] = ft_strdup(args[i]);
		if (!copy[i])
		{
			j = 0;
			while (j < i)
			{
				free(copy[j]);
				j++;
			}
			free(copy);
			return (NULL);
		}
		i++;
	}
	copy[count] = NULL;
	return (copy);
}

int	safe_execute_command(char **args, t_shell *shell)
{
	char	*cmd_path;

	if (!args || !args[0])
	{
		shell->exit_status = 0;
		return (1);
	}
	if (execute_builtin(args, shell))
		exit(shell->exit_status);
	cmd_path = find_command(args[0], shell);
	if (cmd_path)
	{
		execve(cmd_path, args, shell->envp);
		ft_puterr("minishell: ", args[0], CNF, "\n");
		shell->exit_status = 127;
		free(cmd_path);
		// free_shell_resources(shell); // FREE SHELL RESOURCES
		// exit(126);
	}
	else if (ft_strchr(args[0], '/'))
	{
		if (execve(args[0], args, shell->envp) == -1)
		{
			ft_puterr("minishell: ", args[0], CNF, "\n");
			shell->exit_status = 127;
			// free_shell_resources(shell); // FREE SHELL RESOURCES
			// exit(127);
		}
	}
	return (0);
}

int	setup_io_backups(int *original_stdin, int *original_stdout)
{
	*original_stdin = dup(STDIN_FILENO);
	*original_stdout = dup(STDOUT_FILENO);
	if (*original_stdin == -1 || *original_stdout == -1)
	{
		perror("minishell: dup");
		return (-1);
	}
	return (0);
}

int	create_pipe_if_needed(char *next_command, int pipe_fds[2])
{
	if (next_command != NULL)
	{
		if (pipe(pipe_fds) == -1)
		{
			perror("minishell: pipe");
			return (-1);
		}
	}
	return (0);
}

void	close_fds(int original_stdin, int original_stdout, int pipe_fds[2],
		char **args)
{
	if (original_stdin != -1)
		close(original_stdin);
	if (original_stdout != -1)
		close(original_stdout);
	if (pipe_fds[0] != -1)
		close(pipe_fds[0]);
	if (pipe_fds[1] != -1)
		close(pipe_fds[1]);
	free_array(args);
}

int	setup_child_io(int input_fd, int pipe_fds[2])
{
	if (input_fd != STDIN_FILENO && dup2(input_fd, STDIN_FILENO) == -1)
	{
		perror("minishell: dup2 input");
		return (-1);
	}
	if (pipe_fds[1] != -1 && dup2(pipe_fds[1], STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2 output");
		return (-1);
	}
	if (input_fd != STDIN_FILENO)
		close(input_fd);
	if (pipe_fds[0] != -1)
		close(pipe_fds[0]);
	if (pipe_fds[1] != -1)
		close(pipe_fds[1]);
	return (0);
}

int	error_return(char *message, int ret_value)
{
	ft_putstr_fd(message, STDERR_FILENO);
	return (ret_value);
}

int	has_heredoc(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (ft_strncmp(args[i], "<<", 3) == 0)
			return (1);
		i++;
	}
	return (0);
}

void	child_process_work(char **args, int input_fd, int pipe_fds[2],
		t_shell *shell)
{
	char	**args_copy;
	int		redir_status;
	int		heredoc_present;

	signal(SIGPIPE, SIG_DFL);
	heredoc_present = has_heredoc(args);
	if (!heredoc_present)
	{
		if (setup_child_io(input_fd, pipe_fds) != 0)
			exit(1);
	}
	redir_status = handle_redirections(args, shell);
	if (redir_status != 0 || !args[0])
	{
		free_shell_resources(shell);
		if (redir_status != 0)
			exit(shell->exit_status);
		else
			exit(0);
	}
	if (heredoc_present)
	{
		if (setup_child_io(input_fd, pipe_fds) != 0)
			exit(1);
	}
	args_copy = copy_args(args);
	if (!args_copy)
	{
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
		free_shell_resources(shell);
		exit(1);
	}
	safe_execute_command(args_copy, shell);
	free_shell_resources(shell);
	exit(shell->exit_status);
}

void	parent_process_work(int *input_fd, int pipe_fds[2], int original_stdin,
		int original_stdout)
{
	if (pipe_fds[1] != -1)
		close(pipe_fds[1]);
	if (*input_fd != STDIN_FILENO)
		close(*input_fd);
	if (pipe_fds[0] != -1)
		*input_fd = pipe_fds[0];
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdin);
	close(original_stdout);
}

int	is_empty(const char *str)
{
	while (*str)
	{
		if (*str != ' ' && *str != '\t')
			return (0);
		str++;
	}
	return (1);
}

int	process_command(char **commands, int i, int *input_fd, t_shell *shell)
{
	int		pipe_fds[2] = {-1, -1};
	pid_t	pid;
	char	**args;
	int		original_stdin;
	int		original_stdout;

	original_stdin = -1;
	original_stdout = -1;
	if (is_empty(commands[i]))
	{
		args = malloc(sizeof(char *) * 2);
		if (!args)
			return (error_return("minishell: memory allocation error\n", -1));
		args[0] = ft_strdup(commands[i]);
		args[1] = NULL;
	}
	else
		args = split_arguments(commands[i]);
	if (!args)
		return (error_return("minishell: memory allocation error\n", -1));
	if (setup_io_backups(&original_stdin, &original_stdout) == -1
		|| create_pipe_if_needed(commands[i + 1], pipe_fds) == -1)
	{
		close_fds(original_stdin, original_stdout, pipe_fds, args);
		return (-1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		close_fds(original_stdin, original_stdout, pipe_fds, args);
		return (-1);
	}
	if (pid == 0)
		child_process_work(args, *input_fd, pipe_fds, shell);
	else
		parent_process_work(input_fd, pipe_fds, original_stdin,
			original_stdout);
	free_array(args);
	return (pid);
}

void	clean_command_args(char **commands)
{
	int		i;
	char	*cleaned_arg;

	i = 0;
	while (commands[i])
	{
		cleaned_arg = handle_quotes(commands[i]);
		commands[i] = cleaned_arg;
		i++;
	}
}

int	process_commands_in_pipeline(char **commands, int *input_fd, pid_t *pids,
		t_shell *shell)
{
	int		i;
	int		pid_count;
	pid_t	pid;

	i = 0;
	pid_count = 0;
	while (commands[i] != NULL)
	{
		pid = process_command(commands, i, input_fd, shell);
		if (pid == -1)
		{
			if (*input_fd != STDIN_FILENO)
				close(*input_fd);
			return (-1);
		}
		pids[pid_count++] = pid;
		i++;
	}
	return (pid_count);
}

void	wait_for_commands_and_set_status(pid_t *pids, int pid_count,
		t_shell *shell)
{
	int	i;
	int	status;

	i = 0;
	while (i < pid_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == pid_count - 1)
		{
			if (WIFEXITED(status))
				shell->exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				shell->exit_status = 128 + WTERMSIG(status);
		}
		i++;
	}
}

void	append_redirection(char **dest, const char *redir, const char *file)
{
	char	*new_cmd;
	char	*tmp;

	tmp = ft_strjoin(redir, " ");
	if (!tmp)
		return ;
	new_cmd = ft_strjoin(tmp, file);
	free(tmp);
	if (!new_cmd)
		return ;
	tmp = ft_strjoin(*dest, " ");
	if (!tmp)
	{
		free(new_cmd);
		return ;
	}
	free(*dest);
	*dest = ft_strjoin(tmp, new_cmd);
	free(tmp);
	free(new_cmd);
}

int	ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

int	is_redirection_only_command(const char *cmd)
{
	while (*cmd && ft_isspace(*cmd))
		cmd++;
	if ((*cmd == '<' || *cmd == '>') && (*(cmd + 1) == ' ' || ((*cmd == '<'
					|| *cmd == '>') && *(cmd + 1) == *cmd && *(cmd
					+ 2) == ' ')))
	{
		while (*cmd && !ft_isspace(*cmd))
			cmd++;
		while (*cmd && ft_isspace(*cmd))
			cmd++;
		while (*cmd && !ft_isspace(*cmd))
			cmd++;
		while (*cmd && ft_isspace(*cmd))
			cmd++;
		return (*cmd == '\0');
	}
	return (0);
}

char	*extract_command_after_redirection(const char *input)
{
	while (*input && ft_isspace(*input))
		input++;
	if (*input == '<')
	{
		input++;
		if (*input == '<')
			input++;
	}
	while (*input && ft_isspace(*input))
		input++;
	while (*input && !ft_isspace(*input))
		input++;
	while (*input && ft_isspace(*input))
		input++;
	return (ft_strdup(input));
}

static void	handle_redir_only(char **commands, int i, const char *cmd_start)
{
	char		*redir_op;
	char		*filename;
	const char	*ptr;
	const char	*start;

	if (!commands[i + 1])
		return ;
	ptr = cmd_start;
	if (*ptr == '<' && *(ptr + 1) == '<')
		redir_op = ft_strdup("<<");
	else
		redir_op = ft_strdup("<");
	ptr += ft_strlen(redir_op);
	while (*ptr && ft_isspace(*ptr))
		ptr++;
	start = ptr;
	while (*ptr && !ft_isspace(*ptr))
		ptr++;
	filename = ft_substr(start, 0, ptr - start);
	if (redir_op && filename)
	{
		append_redirection(&commands[i + 1], redir_op, filename);
		free(commands[i]);
		commands[i] = ft_strdup("");
	}
	free(redir_op);
	free(filename);
}

static void	handle_inline_redir(char **commands, int i, const char *cmd_start)
{
	char		*redir_op;
	char		*filename;
	char		*cmd_part;
	char		*new_cmd;
	char		*tmp;
	const char	*ptr;
	const char	*start;

	ptr = cmd_start;
	if (*ptr == '<' && *(ptr + 1) == '<')
		redir_op = ft_strdup("<<");
	else
		redir_op = ft_strdup("<");
	ptr += ft_strlen(redir_op);
	while (*ptr && ft_isspace(*ptr))
		ptr++;
	start = ptr;
	while (*ptr && !ft_isspace(*ptr))
		ptr++;
	filename = ft_substr(start, 0, ptr - start);
	while (*ptr && ft_isspace(*ptr))
		ptr++;
	cmd_part = ft_strdup(ptr);
	if (redir_op && filename && cmd_part)
	{
		new_cmd = ft_strjoin(cmd_part, " ");
		tmp = ft_strjoin(new_cmd, redir_op);
		free(new_cmd);
		new_cmd = ft_strjoin(tmp, " ");
		free(tmp);
		tmp = ft_strjoin(new_cmd, filename);
		free(new_cmd);
		free(commands[i]);
		commands[i] = tmp;
	}
	free(redir_op);
	free(filename);
	free(cmd_part);
}

void	preprocess_redirections(char **commands)
{
	int			i;
	const char	*cmd_start;

	i = 0;
	while (commands[i])
	{
		if (!commands[i][0])
		{
			i++;
			continue ;
		}
		cmd_start = commands[i];
		while (*cmd_start && ft_isspace(*cmd_start))
			cmd_start++;
		if (*cmd_start == '<' && cmd_start[1] != '\0')
		{
			if (is_redirection_only_command(commands[i]))
				handle_redir_only(commands, i, cmd_start);
			else
				handle_inline_redir(commands, i, cmd_start);
		}
		i++;
	}
}

void	execute_pipeline(char **commands, t_shell *shell)
{
	int		input_fd;
	pid_t	*pids;
	int		pid_count;

	input_fd = STDIN_FILENO;
	pids = malloc(sizeof(pid_t) * (count_commands(commands) + 1));
	if (!pids)
		return ;
	clean_command_args(commands);
	preprocess_redirections(commands);
	pid_count = process_commands_in_pipeline(commands, &input_fd, pids, shell);
	if (pid_count == -1)
	{
		free(pids);
		return ;
	}
	wait_for_commands_and_set_status(pids, pid_count, shell);
	free(pids);
	if (input_fd != STDIN_FILENO)
		close(input_fd);
}
