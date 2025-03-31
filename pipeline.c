/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:32:02 by carlos-j          #+#    #+#             */
/*   Updated: 2025/03/31 15:06:25 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd, t_builtin *builtins)
{
	int	i;

	i = 0;
	if (!cmd || !builtins || builtins[0].cmd == NULL)
		return (0);
	while (builtins[i].cmd != NULL)
	{
		if (builtins[i].cmd && ft_strncmp(cmd, builtins[i].cmd,
				ft_strlen(builtins[i].cmd) + 1) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	is_executable(char *path)
{
	struct stat	st;

	return (stat(path, &st) == 0 && (st.st_mode & S_IXUSR));
}

char	*find_command_in_path(char *cmd, char **envp)
{
	char	*path_var;
	char	*path_copy;
	char	*dir;
	char	*full_path;
	int		i;

	path_var = NULL;
	path_copy = NULL;
	dir = NULL;
	full_path = NULL;
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_var = envp[i] + 5;
			break ;
		}
		i++;
	}
	if (!path_var)
		return (NULL);
	path_copy = ft_strdup(path_var);
	if (!path_copy)
		return (NULL);
	dir = strtok(path_copy, ":");
	while (dir)
	{
		full_path = malloc(ft_strlen(dir) + ft_strlen(cmd) + 2);
		if (!full_path)
		{
			free(path_copy);
			return (NULL);
		}
		sprintf(full_path, "%s/%s", dir, cmd);
		if (access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (full_path);
		}
		free(full_path);
		dir = strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
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
		return 1;
	}
	if (execute_builtin(args, shell))
		exit(shell->exit_status);

	cmd_path = find_command_in_path(args[0], shell->envp);
	if (cmd_path)
	{
		execve(cmd_path, args, shell->envp);
		free(cmd_path);
	}
	else if (ft_strchr(args[0], '/'))
		execve(args[0], args, shell->envp);
	else
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(args[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		shell->exit_status = 127;
	}
	perror("minishell");
	exit(126);
}

int process_command(char **commands, int i, int *input_fd, t_shell *shell)
{
    int     pipe_fds[2] = {-1, -1};
    pid_t   pid;
    char    **args;
    char    *next_command;
    char    **args_copy;
    int     original_stdin = -1;
    int     original_stdout = -1;
    int     redir_status;

    args = split_arguments(commands[i]);
    if (!args) {
        ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
        return (-1);
    }

    // Save original file descriptors
    original_stdin = dup(STDIN_FILENO);
    original_stdout = dup(STDOUT_FILENO);
    if (original_stdin == -1 || original_stdout == -1) {
        perror("minishell: dup");
        free_array(args);
        return (-1);
    }

    // Set up pipe if there's a next command
    next_command = commands[i + 1];
    if (next_command != NULL) {
        if (pipe(pipe_fds) == -1) {
            perror("minishell: pipe");
            free_array(args);
            close(original_stdin);
            close(original_stdout);
            return (-1);
        }
    }

    pid = fork();
    if (pid == -1) {
        perror("minishell: fork");
        free_array(args);
        close(original_stdin);
        close(original_stdout);
        if (pipe_fds[0] != -1) close(pipe_fds[0]);
        if (pipe_fds[1] != -1) close(pipe_fds[1]);
        return (-1);
    }

    if (pid == 0) {
        // Child process
        close(original_stdin);
        close(original_stdout);

		signal(SIGPIPE, SIG_DFL);  // Reset SIGPIPE to default handling

        // Handle input redirection from previous command
        if (*input_fd != STDIN_FILENO) {
            if (dup2(*input_fd, STDIN_FILENO) == -1) {
                perror("minishell: dup2 input");
                exit(1);
            }
            close(*input_fd);
        }

        // Handle output redirection to next command
        if (next_command != NULL) {
            if (dup2(pipe_fds[1], STDOUT_FILENO) == -1) {
                perror("minishell: dup2 output");
                exit(1);
            }
        }

        // Close pipe ends we don't need
        if (pipe_fds[0] != -1) close(pipe_fds[0]);
        if (pipe_fds[1] != -1) close(pipe_fds[1]);

        // Handle command redirections
        redir_status = handle_redirections(args, shell);
        if (redir_status != 0) {
            exit(shell->exit_status);
        }

        // Check if there's any command left after redirections
        if (!args[0]) {
            exit(0); // No command, just redirections
        }

        args_copy = copy_args(args);
        if (!args_copy) {
            ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
            exit(1);
        }

        safe_execute_command(args_copy, shell);
        ft_putstr_fd("minishell: failed to execute ", STDERR_FILENO);
        ft_putstr_fd(args[0], STDERR_FILENO);
        ft_putstr_fd("\n", STDERR_FILENO);
        exit(1);
    }

    // Parent process
    // Close pipe write end and previous input fd
    if (pipe_fds[1] != -1) close(pipe_fds[1]);
    if (*input_fd != STDIN_FILENO) close(*input_fd);

    // Set up next input fd if needed
    if (next_command != NULL) {
        *input_fd = pipe_fds[0];
    } else {
        if (pipe_fds[0] != -1) close(pipe_fds[0]);
    }

    // Restore original file descriptors
    dup2(original_stdin, STDIN_FILENO);
    dup2(original_stdout, STDOUT_FILENO);
    close(original_stdin);
    close(original_stdout);

    free_array(args);
    return (pid);
}

void execute_pipeline(char **commands, t_shell *shell)
{
    int     input_fd;
    int     i;
    int     status;
    pid_t   pid;
    pid_t   last_pid = -1;  // Initialize to -1
    char    *cleaned_arg;

    input_fd = STDIN_FILENO;
    i = 0;

    // Handle quotes (existing code)
    while (commands[i])
    {
        cleaned_arg = handle_quotes(commands[i]);
        commands[i] = cleaned_arg;
        i++;
    }

    // Execute commands in pipeline
    i = 0;
    while (commands[i] != NULL)
    {
        pid = process_command(commands, i, &input_fd, shell);
        if (pid == -1)
        {
            if (input_fd != STDIN_FILENO)
                close(input_fd);
            return;
        }
        last_pid = pid;  // Track last command
        i++;
    }

    // Wait for last command and get its status
    if (last_pid != -1) {
        waitpid(last_pid, &status, 0);
        if (WIFEXITED(status))
            shell->exit_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            shell->exit_status = 128 + WTERMSIG(status);
    }

    // Clean up any remaining processes (zombies)
    while (waitpid(-1, &status, WNOHANG) > 0) {}

    // Close pipe if still open
    if (input_fd != STDIN_FILENO)
        close(input_fd);
}
