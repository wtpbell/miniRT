/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 14:04:02 by jboon             #+#    #+#             */
/*   Updated: 2025/06/25 21:33:27 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "rt_math.h"
#include "scene.h"
#include "material.h"
#include "libft.h"

static bool	parse_path(int *ctx, const void *raw)
{
	const char	*path = raw;
	char		**dest;
	char		*new_path;

	dest = (char **)ctx;
	if (!path || !*path || !dest)
		return (false);
	new_path = ft_strdup(path);
	if (!new_path)
		return (false);
	if (*dest)
		free(*dest);
	*dest = new_path;
	return (true);
}

static bool	set_texture_pattern(t_mat *mat)
{
	if (mat->tex_path != NULL && mat->tex_path[0] != '\0')
	{
		mat->texture.type = TEX_IMAGE;
		if (!load_texture(&mat->texture, mat->tex_path))
			mat->texture.type = TEX_SOLID;
	}
	if (mat->texture.type == TEX_IMAGE)
		mat->get_texcol = image_pattern;
	else if (mat->texture.type == TEX_CHECKER)
		mat->get_texcol = checker_pattern;
	else
		mat->get_texcol = solid_pattern;
	if (mat->bump_scale > 0.0f && mat->bump_path != NULL
		&& mat->bump_path[0] != '\0')
		load_bump_map(mat, mat->bump_path);
	return (true);
}

static void	init_bump_fields(t_field *fields, int *field_count, t_mat *mat)
{
	const t_field	field_defs[] = {
	{"tex", &mat->tex_path, FIELD_ENUM,
		g_v2f_zero, FILLED, {.to_enum = parse_path}},
	{"pat", &mat->texture.type, FIELD_ENUM,
		g_v2f_zero, FILLED, {.to_enum = str_to_texture_type}},
	{"bump", &mat->bump_path, FIELD_ENUM,
		g_v2f_zero, FILLED, {.to_enum = parse_path}},
	{NULL, NULL, 0, g_v2f_zero, 0, {0}}
	};
	int				i;

	if (!fields || !field_count || !mat)
		return ;
	i = 0;
	while (field_defs[i].name != NULL)
	{
		fields[*field_count] = field_defs[i];
		(*field_count)++;
		i++;
	}
}

static bool	parse_type_material(t_mat *mat, t_mat_type type, char **tokens)
{
	const t_v2f	lim01 = init_v2f(0.0f, 1.0f);
	const t_v2f	lim_shi = init_v2f(0.0f, 5000.0f);
	const t_v2f	lim_ir = init_v2f(0.0f, 100.0f);
	int			count;
	t_field		fields[20];

	count = 0;
	mat->type = type;
	fields[count++] = init_field("spc", &mat->lamb.specular, FIELD_FLT, lim01);
	fields[count++] = init_field("shi", &mat->lamb.shininess, FIELD_FLT, lim_shi);
	fields[count++] = init_field("l_rough", &mat->lamb.roughness, FIELD_FLT, lim01);
	fields[count++] = init_field("mt_rough", &mat->metal.roughness, FIELD_FLT, lim01);
	fields[count++] = init_field("ir", &mat->diel.ir, FIELD_FLT, lim_ir);
	fields[count++] = init_field("tr", &mat->diel.transmittance, FIELD_FLT, lim01);
	fields[count++] = init_field("d_rough", &mat->diel.roughness, FIELD_FLT, lim01);
	fields[count++] = init_field("alb", &mat->albedo, FIELD_COL, lim01);
	init_bump_fields(fields, &count, mat);
	fields[count++] = init_field("bump_scale", &mat->bump_scale, FIELD_FLT, init_v2f(0.0f, 100.0f));
	fields[0].state |= (HIDDEN * (type != MAT_LAMBERTIAN));
	fields[1].state |= (HIDDEN * (type != MAT_LAMBERTIAN));
	fields[2].state |= (HIDDEN * (type != MAT_LAMBERTIAN));
	fields[3].state |= (HIDDEN * (type != MAT_METAL));
	fields[4].state |= (HIDDEN * (type != MAT_DIELECTRIC));
	fields[5].state |= (HIDDEN * (type != MAT_DIELECTRIC));
	fields[6].state |= (HIDDEN * (type != MAT_DIELECTRIC));
	fields[7].state = FILLED;
	init_texture_fields(fields + count, &mat->texture);
	count += 5;
	if (!parse_fields(fields, count, tokens))
		return (false);
	if (mat->tex_path == NULL && mat->texture.type != TEX_CHECKER)
		mat->texture.type = TEX_SOLID;
	return (true);
}

bool	parse_material(char **tokens, t_scene *scene)
{
	t_mat		*mat;
	t_mat_type	mat_type;
	const char	*str_type;

	str_type = NULL;
	if (!is_valid_material_name(tokens[0]))
		return (print_error(ERR_INV_MAT_NAME, "material", tokens[0]), false);
	if (tokens[1] == NULL || !is_field(tokens[1], "type", &str_type))
		return (print_error(ERR_REQ_FIELD, "material", "type:<value>"), false);
	mat_type = get_mat_type(str_type);
	if (mat_type == MAT_UNKNOWN)
		return (print_error(ERR_UNKNOWN_MAT_TYPE, "material", tokens[1]), false);
	mat = find_or_create_material(&scene->shared_materials, tokens[0]);
	if (mat == NULL)
		return (print_error(ERR_MEM, "material", NULL), false);
	if (!parse_type_material(mat, mat_type, tokens + 2))
		return (false);
	if (!set_texture_pattern(mat))
		return (false);
	return (true);
}
