/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:13:07 by carlos-j          #+#    #+#             */
/*   Updated: 2025/02/28 15:10:44 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_exit_status = 0; // ?

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
		return ;
	i = 0;
	while (commands[i])
		free(commands[i++]);
	free(commands);
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
	return (g_exit_status);
}
