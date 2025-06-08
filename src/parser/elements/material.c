/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   material.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/05 14:04:02 by jboon         #+#    #+#                 */
/*   Updated: 2025/06/07 23:24:30 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static inline t_field	init_field(const char *name, void *mem, t_f_type type,
	t_v2f lim)
{
	return ((t_field){name, mem, type, lim, EMPTY | REQUIRED});
}

static bool	parse_type_material(t_mat *mat, t_mat_type type, char **tokens)
{
	const t_v2f	lim_01 = init_v2f(0.0f, 1.0f);
	const t_v2f	lim_spc = init_v2f(0.0f, 5000.0f);
	const t_v2f	lim_ir = init_v2f(0.0f, 100.0f);
	t_field		fields[6];

	mat->type = type;
	fields[0] = init_field("spc", &mat->lamb.specular, FIELD_FLOAT, lim_spc);
	fields[1] = init_field("shi", &mat->lamb.shininess, FIELD_FLOAT, lim_01);
	fields[2] = init_field("fuz", &mat->metal.fuzz, FIELD_FLOAT, lim_01);
	fields[3] = init_field("ir", &mat->diel.ir, FIELD_FLOAT, lim_ir);
	fields[4] = init_field("tr", &mat->diel.transmittance, FIELD_FLOAT, lim_01);
	fields[5] = init_field("alb", &mat->albedo, FIELD_COL, lim_01);
	fields[0].state ^= (REQUIRED * (type != MAT_LAMBERTIAN));
	fields[1].state ^= (REQUIRED * (type != MAT_LAMBERTIAN));
	fields[2].state ^= (REQUIRED * (type != MAT_METAL));
	fields[3].state ^= (REQUIRED * (type != MAT_DIELECTRIC));
	fields[4].state ^= (REQUIRED * (type != MAT_DIELECTRIC));
	fields[5].state ^= REQUIRED;
	fields[0].state |= (HIDDEN * (type != MAT_LAMBERTIAN));
	fields[1].state |= (HIDDEN * (type != MAT_LAMBERTIAN));
	fields[2].state |= (HIDDEN * (type != MAT_METAL));
	fields[3].state |= (HIDDEN * (type != MAT_DIELECTRIC));
	fields[4].state |= (HIDDEN * (type != MAT_DIELECTRIC));
	return (parse_fields(fields, 6, tokens));
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
	{
		print_error(ERR_UNKNOWN_MAT_TYPE, "material", tokens[1]);
		return (false);
	}
	mat = find_or_create_material(&scene->shared_materials, tokens[0]);
	if (mat == NULL)
		return (print_error(ERR_MEM, "material", NULL), false);
	return (parse_type_material(mat, mat_type, tokens + 2));
}
