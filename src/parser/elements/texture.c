/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   texture.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/16 10:29:24 by jboon         #+#    #+#                 */
/*   Updated: 2025/06/16 11:34:49 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static inline t_field	init_field(const char *name, void *mem, t_f_type type,
	t_v2f lim)
{
	return ((t_field){name, mem, type, lim, EMPTY, {NULL}});
}

bool	str_to_texture_type(int *val, const void *enum_name)
{
	if (ft_strcmp("solid", enum_name) == 0)
		return (*val = TEX_SOLID, true);
	else if (ft_strcmp("checker", enum_name) == 0)
		return (*val = TEX_CHECKER, true);
	return (*val = -1, false);
}

bool	handle_texture_fields(t_texture *tex, char **tokens)
{
	t_field	tex_fields[4];

	tex_fields[0] = init_field("type", &tex->type, FIELD_ENUM, init_v2f(0, 1));
	tex_fields[0].to_enum = str_to_texture_type;
	tex_fields[1] = init_field("su", &tex->scale_uv.u, FIELD_FLOAT, init_v2f(0, 1000));
	tex_fields[2] = init_field("sv", &tex->scale_uv.v, FIELD_FLOAT, init_v2f(0, 1000));
	tex_fields[3] = init_field("alt_col", &tex->col, FIELD_COL, init_v2f(0, 1));
	return (parse_fields(tex_fields, 4, tokens, NULL));
}
