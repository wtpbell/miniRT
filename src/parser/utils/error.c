/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 16:24:01 by bewong            #+#    #+#             */
/*   Updated: 2025/06/10 15:24:11 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#define STDERR STDERR_FILENO

static const char *const	g_err_msg[ERR_COUNT] = {
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
[ERR_MISSING_COMPONENT] = "Component is missing",
[ERR_FORMAT] = "Repeated comma between numbers",
[ERR_INV_MAT_NAME] = "Invalid material name (must be ^m_[a-zA-Z0-9]+$)",
[ERR_UNKNOWN_MAT_TYPE] = "Unknown material type",
[ERR_UNKNOWN_FIELD] = "Unknown field (<field>:<value>)",
[ERR_REQ_FIELD] = "Required field is missing",
[ERR_LOAD_TEXTURE] = "Failed to load texture"
};

static const char	*get_err_msg(t_error type)
{
	if (type >= 0 && type < ERR_COUNT)
		return (g_err_msg[type]);
	return ("Unknown error");
}

void	print_error(t_error type, const char *ctx, const char *value)
{
	ft_putstr_fd(RED, STDERR);
	ft_putstr_fd("Error: ", STDERR);
	ft_putstr_fd(RESET, STDERR);
	if (ctx)
	{
		ft_putstr_fd(GREEN, STDERR);
		ft_putstr_fd("[", STDERR);
		ft_putstr_fd((char *)ctx, STDERR);
		ft_putstr_fd("] ", STDERR);
		ft_putstr_fd(RESET, STDERR);
	}
	ft_putstr_fd(RED, STDERR);
	ft_putstr_fd((char *)get_err_msg(type), STDERR);
	if (value)
	{
		ft_putstr_fd(": ", STDERR);
		ft_putstr_fd((char *)value, STDERR);
	}
	ft_putstr_fd("\n", STDERR);
	ft_putstr_fd(RESET, STDERR);
}

void	exit_err(t_error type, const char *ctx, const char *value)
{
	print_error(type, ctx, value);
	exit(type);
}
