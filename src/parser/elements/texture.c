/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   texture.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/16 10:29:24 by jboon         #+#    #+#                 */
/*   Updated: 2025/07/25 14:33:02 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static bool	str_to_texture_type(int *val, const void *enum_name)
{
	if (ft_strcmp("solid", enum_name) == 0)
		return (*val = TEX_SOLID, true);
	if (ft_strcmp("checker", enum_name) == 0)
		return (*val = TEX_CHECKER, true);
	if (ft_strcmp("image", enum_name) == 0)
		return (*val = TEX_IMAGE, true);
	else if (ft_strcmp("perlin", enum_name) == 0)
		return (*val = TEX_PERLIN);
	return (*val = -1, false);
}

static bool	parse_path(int *ctx, const void *raw)
{
	const char	*path = raw;
	char		**dest;
	char		*new_path;

	dest = (char **)ctx;
	if (str_is_empty(path))
		return (false);
	new_path = ft_strdup(path);
	if (!new_path)
		return (false);
	if (*dest)
		free(*dest);
	*dest = new_path;
	return (true);
}

void	init_bump_fields(t_field *fields, int *field_count, t_mat *mat)
{
	const t_field	field_defs[] = {
	{"tex", &mat->tex_path, FIELD_ENUM,
		g_v2f_zero, FILLED, {.to_enum = parse_path}},
	{"pat", &mat->texture.type, FIELD_ENUM,
		g_v2f_zero, FILLED, {.to_enum = str_to_texture_type}},
	{"bump", &mat->bump_path, FIELD_ENUM,
		g_v2f_zero, FILLED, {.to_enum = parse_path}},
	{"bump_scale", &mat->bump_scale, FIELD_FLT,
		init_v2f(0.0f, 1024.0f), EMPTY, {NULL}},
	{NULL, NULL, 0, g_v2f_zero, 0, {0}}
	};
	int				i;

	i = 0;
	while (field_defs[i].name != NULL)
	{
		fields[*field_count] = field_defs[i];
		(*field_count)++;
		i++;
	}
}

void	init_texture_fields(t_field *fields, int *field_count, t_mat *mat)
{
	const t_v2f		lim01 = init_v2f(0, 1);
	const t_v2f		limsv = init_v2f(0, 1024);
	const t_v2f		limtheta = init_v2f(-180, 180);
	const t_field	field_defs[] = {
	{"su", &mat->texture.scale_rot.u, FIELD_FLT, limsv, FILLED, {0}},
	{"sv", &mat->texture.scale_rot.v, FIELD_FLT, limsv, FILLED, {0}},
	{"rot", &mat->texture.scale_rot.z, FIELD_FLT, limtheta, FILLED, {0}},
	{"col", &mat->texture.col, FIELD_COL, lim01, FILLED, {0}},
	{NULL, NULL, 0, g_v2f_zero, 0, {0}}
	};
	int				i;

	i = 0;
	while (field_defs[i].name != NULL)
	{
		fields[*field_count] = field_defs[i];
		(*field_count)++;
		i++;
	}
}
