/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 16:24:01 by bewong        #+#    #+#                 */
/*   Updated: 2025/05/13 17:33:33 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static const char	*get_err_msg(t_error type)
{
	const char	*err_msg[ERR_COUNT] = {
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
	[ERR_POSITIVE_VALUE] = "Value must be positive",
	[ERR_PARSE_FAIL] = "Map parsing failed",
	[ERR_INF] = "Floating point is infinitive"
	};

	if (type >= 0 && type < ERR_COUNT)
		return (err_msg[type]);
	return ("Unknown error");
}

void	print_error(t_error type, const char *ctx, const char *value)
{
	ft_putstr_fd(RED, 2);
	ft_putstr_fd("Error: ", 2);
	ft_putstr_fd(RESET, 2);
	if (ctx)
	{
		ft_putstr_fd(GREEN, 2);
		ft_putstr_fd("[", 2);
		ft_putstr_fd((char *)ctx, 2);
		ft_putstr_fd("] ", 2);
		ft_putstr_fd(RESET, 2);
	}
	ft_putstr_fd(RED, 2);
	ft_putstr_fd((char *)get_err_msg(type), 2);
	if (value)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd((char *)value, 2);
	}
	ft_putstr_fd("\n", 2);
	ft_putstr_fd(RESET, 2);
}

void	exit_err(t_error type, const char *ctx, const char *value)
{
	print_error(type, ctx, value);
	exit(type);
}
