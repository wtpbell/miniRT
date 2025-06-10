/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 14:04:02 by jboon             #+#    #+#             */
/*   Updated: 2025/06/10 20:50:17 by bewong           ###   ########.fr       */
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
	const t_v2f	lim_shi = init_v2f(0.0f, 5000.0f);
	const t_v2f	lim_ir = init_v2f(0.01f, 100.0f);
	t_field		fields[10];
	int			field_count;

	mat->type = type;
	field_count = 0;
	
	// Common fields
	if (ft_strarr_has(tokens, "alb:"))
		fields[field_count++] = init_field("alb", &mat->albedo, FIELD_COL, lim_01);

	// Type-specific fields
	if (type == MAT_LAMBERTIAN)
	{
		if (ft_strarr_has(tokens, "spc:"))
			fields[field_count++] = init_field("spc", &mat->lamb.specular, FIELD_FLOAT, lim_01);
		if (ft_strarr_has(tokens, "shi:"))
			fields[field_count++] = init_field("shi", &mat->lamb.shininess, FIELD_FLOAT, lim_shi);
		if (ft_strarr_has(tokens, "rough:"))
			fields[field_count++] = init_field("rough", &mat->lamb.roughness, FIELD_FLOAT, lim_01);
	}
	else if (type == MAT_METAL)
	{
		if (ft_strarr_has(tokens, "fuz:"))
			fields[field_count++] = init_field("fuz", &mat->metal.fuzz, FIELD_FLOAT, lim_01);
		if (ft_strarr_has(tokens, "rough:"))
			fields[field_count++] = init_field("rough", &mat->metal.roughness, FIELD_FLOAT, lim_01);
	}
	else if (type == MAT_DIELECTRIC)
	{
		if (ft_strarr_has(tokens, "ir:"))
			fields[field_count++] = init_field("ir", &mat->diel.ir, FIELD_FLOAT, lim_ir);
		if (ft_strarr_has(tokens, "tr:"))
			fields[field_count++] = init_field("tr", &mat->diel.transmittance, FIELD_FLOAT, lim_01);
		if (ft_strarr_has(tokens, "rough:"))
			fields[field_count++] = init_field("rough", &mat->diel.roughness, FIELD_FLOAT, lim_01);
	}

	return (parse_fields(fields, field_count, tokens));
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
