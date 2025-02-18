/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:13:07 by carlos-j          #+#    #+#             */
/*   Updated: 2025/02/18 14:29:23 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/*
❌ TODO:

✔️•Display a prompt when waiting for a new command.

✔️•Have a working history.

•Search and launch the right executable (based on the PATH variable or using a
relative or an absolute path).

•Avoid using more than one global variable to indicate a received signal. Consider
the implications: this approach ensures that your signal handler will not access your
main data structures.

•Not interpret unclosed quotes or special characters which are not required by the
subject such as \ (backslash) or ; (semicolon).

•Handle ’ (single quote) which should prevent the shell from interpreting the meta-
characters in the quoted sequence.

•Handle " (double quote) which should prevent the shell from interpreting the meta-
characters in the quoted sequence except for $ (dollar sign).

•Implement redirections:
	◦< should redirect input.
	◦> should redirect output.
	◦<< should be given a delimiter, then read the input until a line containing the
	delimiter is seen. However, it doesn’t have to update the history!
	◦>> should redirect output in append mode.

•Implement pipes (| character). The output of each command in the pipeline is
connected to the input of the next command via a pipe.

•Handle environment variables ($ followed by a sequence of characters) which
should expand to their values.

•Handle $? which should expand to the exit status of the most recently executed
foreground pipeline.

•Handle ctrl-C, ctrl-D and ctrl-\ which should behave like in bash.

•In interactive mode:
	◦ctrl-C displays a new prompt on a new line.
	◦ctrl-D exits the shell.
	◦ctrl-\ does nothing.

•Your shell must implement the following
	◦echo with option -n
	✔️◦cd with only a relative or absolute path
	✔️◦pwd with no options
	◦export with no options
	◦unset with no options
	✔️◦env with no options or arguments
	✔️◦exit with no options

*/

void	build_prompt(char *prompt, const char *user, const char *hostname, const char *display_path)
{
	size_t	i;

	i = 0;
	i += ft_strlcpy(prompt + i, BOLD GREEN "[minishell]" RESET " ", PROMPT_MAX - i);
	i += ft_strlcpy(prompt + i, BOLD BLUE, PROMPT_MAX - i);
	i += ft_strlcpy(prompt + i, user, PROMPT_MAX - i);
	i += ft_strlcpy(prompt + i, "@", PROMPT_MAX - i);
	i += ft_strlcpy(prompt + i, hostname, PROMPT_MAX - i);
	i += ft_strlcpy(prompt + i, RESET ":", PROMPT_MAX - i);
	i += ft_strlcpy(prompt + i, display_path, PROMPT_MAX - i);
	i += ft_strlcpy(prompt + i, GREEN "$" RESET " ", PROMPT_MAX - i);
}


void	handle_input(char *input, char **envp)
{
	char	**args;

	args = ft_split(input, ' ');
	if (!args || !args[0])
		return;

	// create a function to compare it to all buit-ins
	if (ft_strncmp(args[0], "exit", 5) == 0)
	{
		free(args);
		exit(0);
	}
	else if (ft_strncmp(args[0], "cd", 3) == 0)
	{
		builtin_cd(args);
		free(args);
		return;
	}
	else if (ft_strncmp(args[0], "pwd", 4) == 0)
	{
		builtin_pwd();
		free(args);
		return;
	}
	else if (ft_strncmp(args[0], "echo", 5) == 0)
	{
		builtin_echo(args);
		free(args);
		return;
	}
	execute_command(args, envp);
	free(args);
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	char	*user;
	char	hostname[HOSTNAME_MAX];
	char	cwd[PATH_MAX];
	char	prompt[PROMPT_MAX];
	char	*home;
	char	*display_path;

	(void)argv;
	if (argc != 1)
	{
		ft_putstr_fd("error: minishell should not have any arguments.\n", 2);
		return (1);
	}
	user = getenv("USER");
	if (!user)
		user = "unknown";
	if (gethostname(hostname, HOSTNAME_MAX) != 0)
		ft_strlcpy(hostname, "unknown", HOSTNAME_MAX);
	home = getenv("HOME");
	if (!home)
		home = "";
	while (1)
	{
		if (getcwd(cwd, sizeof(cwd)) == NULL)
		{
			perror("getcwd() error");
			ft_strlcpy(cwd, "unknown", sizeof(cwd));
		}
		display_path = shorten_path(cwd, home);
		build_prompt(prompt, user, hostname, display_path);
		input = readline(prompt);
		if (!input)
			break;
		if (*input)
		{
			add_history(input);
			handle_input(input, envp);
		}
		free(input);
	}
	return (0);
}
