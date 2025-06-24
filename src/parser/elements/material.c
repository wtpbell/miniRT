/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   material.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/05 14:04:02 by jboon         #+#    #+#                 */
/*   Updated: 2025/07/24 12:02:04 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "rt_math.h"
#include "scene.h"
#include "material.h"
#include "libft.h"

static void	set_texture_pattern(t_mat *mat)
{
	if (mat->texture.type == TEX_CHECKER)
		mat->get_texcol = checker_pattern;
	else if (mat->texture.type == TEX_IMAGE)
		mat->get_texcol = image_pattern;
	else if (mat->texture.type == TEX_PERLIN)
		mat->get_texcol = value_noise_pattern;
	else
		mat->get_texcol = solid_pattern;
}

static void	init_material_fields(t_field *fields, int *field_count, t_mat *mat)
{
	const t_v2f		lim01 = init_v2f(0.0f, 1.0f);
	const t_v2f		lim_shi = init_v2f(0.0f, 5000.0f);
	const t_v2f		lim_ir = init_v2f(0.0f, 100.0f);
	const t_field	field_defs[] = {
		init_field("spc", &mat->lamb.specular, FIELD_FLT, lim01),
		init_field("shi", &mat->lamb.shininess, FIELD_FLT, lim_shi),
		init_field("l_rough", &mat->lamb.roughness, FIELD_FLT, lim01),
		init_field("mt_rough", &mat->metal.roughness, FIELD_FLT, lim01),
		init_field("ir", &mat->diel.ir, FIELD_FLT, lim_ir),
		init_field("tr", &mat->diel.transmittance, FIELD_FLT, lim01),
		init_field("d_rough", &mat->diel.roughness, FIELD_FLT, lim01),
		init_field("alb", &mat->albedo, FIELD_COL, lim01),
		init_field(NULL, NULL, 0, (t_v2f){{0}})
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

static bool	parse_type_material(t_mat *mat, t_mat_type type, char **tokens)
{
	int		count;
	t_field	fields[20];

	count = 0;
	mat->type = type;
	init_material_fields(fields, &count, mat);
	init_bump_fields(fields, &count, mat);
	init_texture_fields(fields, &count, mat);
	fields[0].state |= (HIDDEN * (type != MAT_LAMBERTIAN));
	fields[1].state |= (HIDDEN * (type != MAT_LAMBERTIAN));
	fields[2].state |= (HIDDEN * (type != MAT_LAMBERTIAN));
	fields[3].state |= (HIDDEN * (type != MAT_METAL));
	fields[4].state |= (HIDDEN * (type != MAT_DIELECTRIC));
	fields[5].state |= (HIDDEN * (type != MAT_DIELECTRIC));
	fields[6].state |= (HIDDEN * (type != MAT_DIELECTRIC));
	fields[7].state = FILLED;
	if (!parse_fields(fields, count, tokens))
		return (false);
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
		return (print_error(ERR_UNKNOWN_MAT, "material", tokens[1]), false);
	mat = find_or_create_material(&scene->shared_materials, tokens[0]);
	if (mat == NULL)
		return (print_error(ERR_MEM, "material", NULL), false);
	if (!parse_type_material(mat, mat_type, tokens + 2))
		return (false);
	assign_textures(mat);
	set_texture_pattern(mat);
	return (true);
}
