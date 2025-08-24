/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_error.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/23 17:32:13 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/23 17:38:01 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_ERROR_H
# define RT_ERROR_H

# define RED	"\033[31m"
# define GREEN	"\033[32m"
# define YELLOW	"\033[33m"
# define BLUE	"\033[34m"
# define RESET	"\033[0m"

typedef enum e_error
{
	ERR_NONE,
	ERR_NUM_ARGS,
	ERR_FILE_FORMAT,
	ERR_RANGE,
	ERR_MEM,
	ERR_FILE_READ,
	ERR_UNKNOWN_TOKEN,
	ERR_FILE_NONEXIST,
	ERR_STOI,
	ERR_STOF,
	ERR_SPHERE_ARGS,
	ERR_TOKEN_COUNT,
	ERR_DUPLICATE,
	ERR_INVALID_VALUE,
	ERR_V3F,
	ERR_POSITIVE_VALUE,
	ERR_PARSE_FAIL,
	ERR_INF,
	ERR_MISSING_COMPONENT,
	ERR_FORMAT,
	ERR_INV_MAT_NAME,
	ERR_UNKNOWN_MAT,
	ERR_UNKNOWN_FIELD,
	ERR_REQ_FIELD,
	ERR_LOAD_TEXTURE,
	ERR_OBJ_VTX_FACE,
	ERR_OBJ_FACE_FORMAT,
	ERR_OBJ_VERT_INDEX,
	ERR_OBJ_VERT_COMP,
	ERR_OBJ_FAIL,
	ERR_OBJ_FACE,
	ERR_COUNT
}	t_error;

void	sys_error(const char *ctx);
void	rt_mlx_error(const char *ctx);
void	rt_error(t_error type, const char *ctx, const char *value);

#endif
