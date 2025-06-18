/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   texture.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/16 10:29:24 by jboon         #+#    #+#                 */
/*   Updated: 2025/06/18 17:32:57 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

bool	str_to_texture_type(int *val, const void *enum_name)
{
	if (ft_strcmp("solid", enum_name) == 0)
		return (*val = TEX_SOLID, true);
	else if (ft_strcmp("checker", enum_name) == 0)
		return (*val = TEX_CHECKER, true);
	return (*val = -1, false);
}

void	init_texture_fields(t_field *tex_fields, t_tex *tex)
{
	const t_v2f	lim01 = init_v2f(0, 1);
	const t_v2f	limsv = init_v2f(0, 1000);
	const t_v2f	limtheta = init_v2f(-180, 180);

	tex_fields[0] = init_field("pat", &tex->type, FIELD_ENUM, lim01);
	tex_fields[0].to_enum = str_to_texture_type;
	tex_fields[1] = init_field("su", &tex->scale_rot.u, FIELD_FLOAT, limsv);
	tex_fields[2] = init_field("sv", &tex->scale_rot.v, FIELD_FLOAT, limsv);
	tex_fields[3] = init_field("rot", &tex->scale_rot.z, FIELD_FLOAT, limtheta);
	tex_fields[4] = init_field("alt_col", &tex->col, FIELD_COL, lim01);
}
