/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:13:07 by carlos-j          #+#    #+#             */
/*   Updated: 2025/03/10 15:00:50 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//int		g_exit_status = 0; // ?

volatile sig_atomic_t g_signal_status = 0; // ✅ Only global allowed

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

void handle_signal(int sig)
{
    if (sig == SIGINT) // Ctrl+C (Interrupt)
    {
        g_signal_status = 1; // Mark that SIGINT was received
        write(1, "\n", 1); // Move to a new line
        rl_on_new_line(); // Reset Readline
        rl_replace_line("", 0); // Clear input line
        rl_redisplay(); // Redisplay prompt after new line
    }
    else if (sig == SIGQUIT) // Ctrl+\ (Quit)
    {
        write(1, "\b\b  \b\b", 6); // Ignore SIGQUIT (like Bash)
    }
}


int main(int argc, char **argv, char **envp)
{
    char *input;
    char *user;
    char hostname[HOSTNAME_MAX];
    char cwd[PATH_MAX];
    char prompt[PROMPT_MAX];
    char *home;
    char *display_path;
    int exit_status;

    exit_status = 0;
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
    signal(SIGINT, handle_signal);
    signal(SIGQUIT, handle_signal);
	g_signal_status = 0; // moved here
    while (1)
    {
        //g_signal_status = 0;

        if (getcwd(cwd, sizeof(cwd)) == NULL)
        {
            perror("getcwd() error");
            ft_strlcpy(cwd, "unknown", sizeof(cwd));
        }
        display_path = shorten_path(cwd, home);
        if (!g_signal_status)
            build_prompt(prompt, user, hostname, display_path);
		else
		{
			g_signal_status = 0;
			prompt[0] = '\0';
		}
        input = readline(prompt);
        if (!input)
        {
            printf("exit\n");
            break ;
        }
        if (*input)
        {
            add_history(input);
            handle_input(input, envp, &exit_status);
        }
        free(input);
    }
    return (exit_status);
}
