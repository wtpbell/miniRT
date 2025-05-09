/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_error.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/09 09:45:51 by bewong        #+#    #+#                 */
/*   Updated: 2025/05/09 18:23:31 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_error	*error(void)
{
	static t_error error = ERR_NO;
	
	return (&error);
}

static void	write_err(const char *err_msg[], const char *msg)
{
	if (*error() == ERR_NO)
	{
		if (msg)
			printf("%s\n", msg);
	}
	else
	{
		if (msg)
			printf("%s: %s\n", msg, err_msg[*error()]);
		else
			printf("%s\n", err_msg[*error()]);
	}
}


void	print_err(const char *msg)
{
	const char *err_msg[ERR_COUNT] = {
		[ERR_NO] = "No error",
		[ERR_NUM_ARGS] = "Number of args must be only 2",
		[ERR_FILE_FORMAT] = "The file format must be .rt",
		[ERR_RANGE] = "The input number is out of range",
		[ERR_MEM] = "Memory allocation fail",
		[ERR_FILE_READ] = "File cannot be read",
		[ERR_UNKNOWN_TOKEN] = "Unknown token is found",
		[ERR_FILE_NONEXIST] = "File is non-exist"
	};
	write_err(err_msg, msg);
}

void	exit_err(t_error type, const char *msg)
{
	*error() = type;
	print_err(msg);
	exit(type);
}
