/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 19:10:30 by pesoares          #+#    #+#             */
/*   Updated: 2025/05/29 02:24:26 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void cleanup_temp_file(void *filename_ptr)
{
    char *filename = (char *)filename_ptr;

    if (filename)
    {
        unlink(filename);
        free(filename);
    }
}

void cleanup_all_temp_files(t_shell *shell)
{
    if (!shell || !shell->temp_files)
        return;

    ft_lstclear(&shell->temp_files, cleanup_temp_file);
    shell->temp_files = NULL;
}

int	is_redirection_token(char *token)
{
	return ((ft_strncmp(token, "<", 1) == 0 && ft_strlen(token) == 1)
		|| (ft_strncmp(token, "<<", 2) == 0 && ft_strlen(token) == 2)
		|| (ft_strncmp(token, ">", 1) == 0 && ft_strlen(token) == 1)
		|| (ft_strncmp(token, ">>", 2) == 0 && ft_strlen(token) == 2));
}

int	ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

char	*ft_strndup(const char *s, size_t n)
{
	char	*dup;
	size_t	len;

	len = ft_strlen(s);
	if (n < len)
		len = n;
	dup = malloc(len + 1);
	if (!dup)
		return (NULL);
	ft_memcpy(dup, s, len);
	dup[len] = '\0';
	return (dup);
}

void	restore_stdio(int out, int in)
{
	dup2(out, STDOUT_FILENO);
	dup2(in, STDIN_FILENO);
	close(out);
	close(in);
}

void close_all_fds()
{
	int i = 3;
	while (i < 1024)
		close(i++);
}

void	handle_signal_status(t_shell *shell)
{
	if (g_signal_status)
	{
		shell->exit_status = 130;
		g_signal_status = 0;
	}
}
