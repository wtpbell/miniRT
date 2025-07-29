/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   texture.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/16 10:29:24 by jboon         #+#    #+#                 */
/*   Updated: 2025/07/29 12:02:07 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static bool	str_to_texture_type(int *val, const void *enum_name)
{
	static const char		*str_types[] = {"solid", "checker", "image",
		"perlin", "pink", "wood", "turb", "marble", NULL};
	static const t_tex_type	types[] = {TEX_SOLID, TEX_CHECKER, TEX_IMAGE,
		TEX_PERLIN, TEX_PINK, TEX_WOOD, TEX_TURB, TEX_MARB};
	int						i;

	i = 0;
	while (str_types[i] != NULL)
	{
		if (ft_strcmp(str_types[i], enum_name) == 0)
			return (*val = types[i], true);
		++i;
	}
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
	{"tex", &mat->texture.tex_path, FIELD_ENUM,
		g_v2f_zero, FILLED, {.to_enum = parse_path}},
	{"pat", &mat->texture.type, FIELD_ENUM,
		g_v2f_zero, FILLED, {.to_enum = str_to_texture_type}},
	{"bump_pat", &mat->bump_map.type, FIELD_ENUM,
		g_v2f_zero, FILLED, {.to_enum = str_to_texture_type}},
	{"bump", &mat->bump_map.tex_path, FIELD_ENUM,
		g_v2f_zero, FILLED, {.to_enum = parse_path}},
	{"bump_scale", &mat->bump_map.scale, FIELD_FLT,
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
