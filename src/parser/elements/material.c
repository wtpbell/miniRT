/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   material.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/05 14:04:02 by jboon         #+#    #+#                 */
/*   Updated: 2025/06/07 20:40:37 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static inline t_field	init_fmat(const char *name, void *mem, t_f_type type, t_v2f lim)
{
	return (t_field){name, mem, type, lim, EMPTY | REQUIRED};
}

static int	parse_lambertian(t_mat *mat, char **tokens)
{
	t_field	fields[3];

	mat->type = MAT_LAMBERTIAN;
	fields[0] = init_fmat("spc", &mat->lamb.specular, FIELD_FLOAT, init_v2f(0.0f, 1.0f));
	fields[1] = init_fmat("shi", &mat->lamb.shininess, FIELD_FLOAT, init_v2f(0.0, 1.0f));
	fields[2] = init_fmat("alb", &mat->albedo, FIELD_COL, init_v2f(0.0, 1.0f));
	fields[2].state ^= REQUIRED;
	return (parse_fields(fields, 3, tokens));
}

static int	parse_metal(t_mat *mat, char **tokens)
{
	t_field fields[2];

	mat->type = MAT_METAL;
	fields[0] = init_fmat("fuz", &mat->metal.fuzz, FIELD_FLOAT, init_v2f(0.0f, 1.0f));
	fields[1] = init_fmat("alb", &mat->albedo, FIELD_COL, init_v2f(0.0, 1.0f));
	fields[1].state ^= REQUIRED;
	return (parse_fields(fields, 2, tokens));
}

static int	parse_dielectric(t_mat *mat, char **tokens)
{
	t_field	fields[3];

	mat->type = MAT_DIELECTRIC;
	fields[0] = init_fmat("ir", &mat->diel.ir, FIELD_FLOAT, init_v2f(0.0f, 1.0f));
	fields[1] = init_fmat("trs", &mat->diel.transmittance, FIELD_FLOAT, init_v2f(0.0, 1.0f));
	fields[2] = init_fmat("alb", &mat->albedo, FIELD_COL, init_v2f(0.0, 1.0f));
	fields[2].state ^= REQUIRED;
	return (parse_fields(fields, 3, tokens));
}

bool	parse_material(char **tokens, t_scene *scene)
{
	t_mat		*mat;
	t_mat_type	mat_type;
	const char	*str_type;

	str_type = NULL;
	if (!is_valid_material_name(tokens[0]))
		return (print_error(ERR_INV_MAT_NAME, "material" , tokens[0]), false);
	if (tokens[1] == NULL || !is_field(tokens[1], "type", &str_type))
		return (print_error(ERR_REQ_FIELD, "material", "type:<value>"), false);
	mat_type = get_mat_type(str_type);
	if (mat_type == MAT_UNKNOWN)
		return (print_error(ERR_UNKNOWN_MAT_TYPE, "material" , tokens[1]), false);
	mat = find_or_create_material(&scene->shared_materials, tokens[0]);
	if (mat == NULL)
		return (print_error(ERR_MEM, "material", NULL), false);
	return ((mat_type == MAT_LAMBERTIAN && parse_lambertian(mat, tokens + 2))
		|| (mat_type == MAT_METAL && parse_metal(mat, tokens + 2))
		|| (mat_type == MAT_DIELECTRIC && parse_dielectric(mat, tokens + 2)));
}
