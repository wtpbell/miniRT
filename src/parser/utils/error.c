/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 16:24:01 by bewong        #+#    #+#                 */
/*   Updated: 2025/05/12 14:29:17 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_error	*error(void)
{
	static t_error error = ERR_NONE;
	
	return (&error);
}

static void	write_err(const char *err_msg[], const char *msg)
{
	if (*error() == ERR_NONE)
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


bool	print_err(const char *msg)
{
	const char *err_msg[ERR_COUNT] = {
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
		[ERR_INVALID_VALUE] = "Invalid value",
		[ERR_V3F] = "Error v3f parsing",
		[ERR_POSITIVE_VALUE] = "Value must be positive"
	};
	write_err(err_msg, msg);
	return (false);
}

void	exit_err(t_error type, const char *msg)
{
	*error() = type;
	print_err(msg);
	exit(type);
}



