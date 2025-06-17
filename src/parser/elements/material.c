/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 14:04:02 by jboon             #+#    #+#             */
/*   Updated: 2025/06/13 13:12:50 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static bool	parse_type_material(t_mat *mat, t_mat_type type, char **tokens)
{
	const t_v2f	lim_01 = init_v2f(0.0f, 1.0f);
	const t_v2f	lim_shi = init_v2f(0.0f, 5000.0f);
	const t_v2f	lim_ir = init_v2f(0.0f, 100.0f);
	t_field		fields[8];

	mat->type = type;
	fields[0] = init_field("spc", &mat->lamb.specular, FIELD_FLOAT, lim_01);
	fields[1] = init_field("shi", &mat->lamb.shininess, FIELD_FLOAT, lim_shi);
	fields[2] = init_field("l_rough", &mat->lamb.roughness, FIELD_FLOAT, lim_01);
	fields[3] = init_field("mt_rough", &mat->metal.roughness, FIELD_FLOAT, lim_01);
	fields[4] = init_field("ir", &mat->diel.ir, FIELD_FLOAT, lim_ir);
	fields[5] = init_field("tr", &mat->diel.transmittance, FIELD_FLOAT, lim_01);
	fields[6] = init_field("d_rough", &mat->diel.roughness, FIELD_FLOAT, lim_01);
	fields[7] = init_field("alb", &mat->albedo, FIELD_COL, lim_01);
	fields[0].state |= (HIDDEN * (type != MAT_LAMBERTIAN));
	fields[1].state |= (HIDDEN * (type != MAT_LAMBERTIAN));
	fields[2].state |= (HIDDEN * (type != MAT_LAMBERTIAN));
	fields[3].state |= (HIDDEN * (type != MAT_METAL));
	fields[4].state |= (HIDDEN * (type != MAT_DIELECTRIC));
	fields[5].state |= (HIDDEN * (type != MAT_DIELECTRIC));
	fields[6].state |= (HIDDEN * (type != MAT_DIELECTRIC));
	fields[7].state = FILLED;
	return (parse_fields(fields, 8, tokens));
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

