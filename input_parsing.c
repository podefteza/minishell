/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:04:57 by carlos-j          #+#    #+#             */
/*   Updated: 2025/05/16 11:00:47 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*void clean_arguments(t_shell *shell)
{
    int     i;
    char    *cleaned_arg;

    if (!shell || !shell->input.args)
        return;

    i = 0;
    while (shell->input.args[i])
    {
        cleaned_arg = handle_quotes(shell->input.args[i]);
        free(shell->input.args[i]);          // Free the original string
        shell->input.args[i] = cleaned_arg;  // Replace with cleaned string
        i++;
    }
}
*/

