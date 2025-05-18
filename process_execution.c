/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 17:34:50 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/16 13:25:51 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*void	handle_child_process(char *full_path, char **args, t_shell *shell)
{
	//printf("child??\n");
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	execve(full_path, args, shell->envp);
	perror("execve");
	shell->exit_status = 1;
	free_shell_resources(shell);
	free_array(args); // added this...
	exit(1);
}

static void handle_parent_process(pid_t pid, int is_background, char **args, t_shell *shell)
{
    int status = 0;  // Initialize status

    if (is_background) {
        shell->last_bg_pid = pid;
        printf("[%d] %s started in background\n", pid, args[0]);
    } else {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            shell->exit_status = WEXITSTATUS(status);
        } else if (WIFSIGNALED(status)) {
            if (WTERMSIG(status) == SIGQUIT) {
                write(STDERR_FILENO, "Quit (core dumped)\n", 20);
            }
            shell->exit_status = 128 + WTERMSIG(status);
        }
    }
}

void	execute_process(char *full_path, char **args, int is_background,
		t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		perror("fork");
	else if (pid == 0)
		handle_child_process(full_path, args, shell);
	else
		handle_parent_process(pid, is_background, args, shell);
}
*/
