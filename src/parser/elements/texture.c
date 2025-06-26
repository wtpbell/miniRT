/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 10:29:24 by jboon             #+#    #+#             */
/*   Updated: 2025/06/26 17:00:11 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

bool	str_to_texture_type(int *val, const void *enum_name)
{
	if (ft_strcmp("solid", enum_name) == 0)
		return (*val = TEX_SOLID, true);
	if (ft_strcmp("checker", enum_name) == 0)
		return (*val = TEX_CHECKER, true);
	if (ft_strcmp("image", enum_name) == 0)
		return (*val = TEX_IMAGE, true);
	return (*val = -1, false);
}

void	init_texture_fields(t_field *tex_fields, t_tex *tex)
{
	const t_v2f		lim01 = init_v2f(0, 1);
	const t_v2f		limsv = init_v2f(0, 1000);
	const t_v2f		limtheta = init_v2f(-180, 180);
	const t_field	field_defs[] = {
	{"su", &tex->scale_rot.u, FIELD_FLT, limsv, FILLED, {0}},
	{"sv", &tex->scale_rot.v, FIELD_FLT, limsv, FILLED, {0}},
	{"rot", &tex->scale_rot.z, FIELD_FLT, limtheta, HIDDEN, {0}},
	{"alt_col", &tex->col, FIELD_COL, lim01, HIDDEN, {0}},
	{"col", &tex->col, FIELD_COL, lim01, HIDDEN, {0}},
	{NULL, NULL, 0, g_v2f_zero, 0, {0}}
	};
	int				i;

	tex->scale_rot = init_v3f(1.0f, 1.0f, 0.0f);
	tex->col = g_v3f_one;
	tex->type = TEX_SOLID;
	i = 0;
	while (field_defs[i].name != NULL)
	{
		tex_fields[i] = field_defs[i];
		i++;
	}
}
