#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <limits.h> // For PATH_MAX
#include <string.h> // For strncmp and strcpy
#include <linux/limits.h>

#define HOSTNAME_MAX 256

// Define ANSI color codes
#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLD    "\033[1m"
#define RESET   "\033[0m"

char *shorten_path(const char *cwd, const char *home)
{
    static char shortened_path[PATH_MAX];

    if (strncmp(cwd, home, strlen(home)) == 0)
	{
        snprintf(shortened_path, sizeof(shortened_path), "~%s", cwd + strlen(home));
    }
	else {
        strncpy(shortened_path, cwd, sizeof(shortened_path));
    }

    return shortened_path;
}

int main(int argc, char **argv, char **envp)
{
    char    *input;
    char    *user;
    char    hostname[HOSTNAME_MAX];
    char    cwd[PATH_MAX];
    char    prompt[HOSTNAME_MAX + PATH_MAX + 64];
    char    *home;

    (void)argv;
	(void)envp;
    if (argc != 1)
    {
        printf("error. minishell should not have any arguments.\n");
        return (1);
    }
    user = getenv("USER");
    if (!user)
        user = "unknown";
    if (gethostname(hostname, HOSTNAME_MAX) != 0)
        snprintf(hostname, HOSTNAME_MAX, "unknown");
    home = getenv("HOME");
    if (!home)
        home = "";
    while (1)
    {
        if (getcwd(cwd, sizeof(cwd)) == NULL)
        {
            perror("getcwd() error");
            snprintf(cwd, sizeof(cwd), "unknown");
        }
        const char *display_path = shorten_path(cwd, home);
        snprintf(prompt, sizeof(prompt), "%s%sminishell%s | %s%s%s@%s%s:%s$ ", BOLD, GREEN, RESET, BOLD, BLUE, user, hostname, RESET, display_path);
        input = readline(prompt);
        if (!input)
            break;
        if (*input)
            add_history(input);
        free(input);
    }

    return (0);
}
