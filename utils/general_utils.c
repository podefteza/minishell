/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 19:10:30 by pesoares          #+#    #+#             */
/*   Updated: 2025/05/30 12:08:07 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	cleanup_all_temp_files(t_shell *shell)
{
	t_list	*current;
	t_list	*next;
	char	*filename;

	if (!shell || !shell->temp_files)
		return ;
	current = shell->temp_files;
	while (current)
	{
		next = current->next;
		filename = (char *)current->content;
		if (filename)
		{
			unlink(filename);
			free(filename);
		}
		free(current);
		current = next;
	}
	shell->temp_files = NULL;
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

void	close_all_fds(void)
{
	int	i;

	i = 3;
	while (i < 1024)
		close(i++);
}
