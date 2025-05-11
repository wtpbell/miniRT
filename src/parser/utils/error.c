/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 16:24:01 by bewong            #+#    #+#             */
/*   Updated: 2025/05/11 18:01:29 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_error	*error(void)
{
	static t_error err = ERR_NONE;
	return (&err);
}

void	set_error(t_error err)
{
	*error() = err;
}

const char	*get_error_message(t_error err)
{
	static const char *messages[] = {
		[ERR_NONE] = "No error",
		[ERR_NUM_ARGS] = "Invalid number of arguments",
		[ERR_FILE_FORMAT] = "Invalid file format",
		[ERR_RANGE] = "Value out of range",
		[ERR_MEM] = "Memory allocation failed",
		[ERR_FILE_READ] = "Error reading file",
		[ERR_UNKNOWN_TOKEN] = "Unknown token type",
		[ERR_FILE_NONEXIST] = "File does not exist",
		[ERR_STOI] = "Invalid integer value",
		[ERR_STOF] = "Invalid float value",
		[ERR_SPHERE_ARGS] = "Invalid sphere arguments",
		[ERR_TOKEN_COUNT] = "Invalid number of tokens",
		[ERR_DUPLICATE] = "Duplicate identifier",
		[ERR_INVALID_VALUE] = "Invalid value"
	};
	return (messages[err]);
}

void	exit_err(t_error type, const char *msg)
{
	if (msg)
		ft_putendl_fd((char *)msg, 2);
	exit(type);
}

void	print_err(const char *msg)
{
	if (msg)
		ft_putendl_fd((char *)msg, 2);
}
