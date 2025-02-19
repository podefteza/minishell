/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:13:07 by carlos-j          #+#    #+#             */
/*   Updated: 2025/02/19 16:37:31 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	build_prompt(char *prompt, const char *user, const char *hostname,
		const char *display_path)
{
	size_t	i;

	i = 0;
	i += ft_strlcpy(prompt + i, BOLD GREEN "[minishell]" RESET " ", PROMPT_MAX
			- i);
	i += ft_strlcpy(prompt + i, BOLD BLUE, PROMPT_MAX - i);
	i += ft_strlcpy(prompt + i, user, PROMPT_MAX - i);
	i += ft_strlcpy(prompt + i, "@", PROMPT_MAX - i);
	i += ft_strlcpy(prompt + i, hostname, PROMPT_MAX - i);
	i += ft_strlcpy(prompt + i, RESET ":", PROMPT_MAX - i);
	i += ft_strlcpy(prompt + i, display_path, PROMPT_MAX - i);
	i += ft_strlcpy(prompt + i, GREEN "$" RESET " ", PROMPT_MAX - i);
}

void	free_args(char **args)
{
	size_t	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
		free(args[i++]);
	free(args);
}

void	free_commands(char **commands)
{
	int	i;

	if (!commands)
		return;

	i = 0;
	while (commands[i])
		free(commands[i++]);
	free(commands);
}


void trim_commands(char **commands)
{
    int i = 0;
    while (commands[i])
    {
        char *trimmed = ft_strtrim(commands[i], " \t\n");
        free(commands[i]);
        commands[i] = trimmed;
        i++;
    }
}

void	handle_input(char *input, char **envp)
{
	t_builtin	builtins[8];
	char		**commands;
	char		**args;
	int			i;
	int			pipe_fds[2];

	builtin_setup(builtins);
	if (ft_strchr(input, '|'))
	{
		commands = ft_split(input, '|');
		if (!commands || !commands[0])
		{
			printf("minishell: syntax error: unexpected '|'\n");
			free_commands(commands);
			return;
		}
		trim_commands(commands);
		fork_processes(pipe_fds, commands, envp);
		free_commands(commands);
		return;
	}
	args = ft_split(input, ' ');
	if (!args || !args[0])
	{
		free_args(args);
		return;
	}
	i = 0;
	while (builtins[i].cmd != NULL)
	{
		if (ft_strncmp(args[0], builtins[i].cmd,
				ft_strlen(builtins[i].cmd)) == 0)
		{
			builtins[i].func(args);
			free_args(args);
			return;
		}
		i++;
	}
	execute_command(args, envp);
	free_args(args);
}

void	get_host_name(char *hostname)
{
	int	fd;
	int	ret;

	fd = open("/etc/hostname", O_RDONLY);
	if (fd < 0)
	{
		ft_strlcpy(hostname, "unknown", HOSTNAME_MAX);
		return ;
	}
	ret = read(fd, hostname, HOSTNAME_MAX - 1);
	close(fd);
	if (ret > 0)
		hostname[ret - 1] = '\0';
	else
		ft_strlcpy(hostname, "unknown", HOSTNAME_MAX);
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
	get_host_name(hostname);
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
			break ;
		if (*input)
		{
			add_history(input);
			handle_input(input, envp);
		}
		free(input);
	}
	return (0);
}
