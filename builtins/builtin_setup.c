/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_setup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlos-j <carlos-j@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:26:04 by carlos-j          #+#    #+#             */
/*   Updated: 2025/02/19 16:42:04 by carlos-j         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	builtin_setup(t_builtin *builtins)
{
	builtins[0].cmd = "echo";
	builtins[0].func = builtin_echo;
	builtins[1].cmd = "cd";
	builtins[1].func = builtin_cd;
	builtins[2].cmd = "pwd";
	builtins[2].func = builtin_pwd;
	builtins[3].cmd = "4242"; // export
	builtins[3].func = builtin_echo; // builtin_export
	builtins[4].cmd = "424242"; // unset
	builtins[4].func = builtin_echo; // builtin_unset
	builtins[5].cmd = "42424242"; // env
	builtins[5].func = builtin_echo; // builtin_env
	builtins[6].cmd = "exit";
	builtins[6].func = builtin_exit;
	builtins[7].cmd = NULL;
	builtins[7].func = NULL;
}
