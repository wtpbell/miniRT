/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   material.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/05 14:04:02 by jboon         #+#    #+#                 */
/*   Updated: 2025/07/31 19:31:39 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "rt_math.h"
#include "scene.h"
#include "material.h"
#include "libft.h"

static void	override_unset_perlin_values(t_perlin *dst, const t_perlin *src)
{
	if (dst->rate == 0.0f)
		dst->rate = src->rate;
	if (dst->gain == 0.0f)
		dst->gain = src->gain;
	if (dst->freq == 0.0f)
		dst->freq = src->freq;
	if (dst->ampt == 0.0f)
		dst->ampt = src->ampt;
	if (dst->layers == 0)
		dst->layers = src->layers;
	if (dst->marble.distortion == 0.0f)
		dst->marble.distortion = src->marble.distortion;
	if (dst->marble.scale == 0.0f)
		dst->marble.scale = src->marble.scale;
}

static void	set_perlin_pattern(t_tex_type *type, t_perlin *dst, t_fp_perlin *fp)
{
	const t_tex_type	types[4] = {TEX_PINK, TEX_WOOD, TEX_MARB, TEX_TURB};
	const t_fp_perlin	fp_perl[4] = {pink_noise, wood_noise, marble_noise, turbulence_noise};
	const t_perlin	p_data[4] = {
		(t_perlin){2.0f, 0.5f, 1.0f, 1.0f, 5, {1.0f, 1.0f}},
		(t_perlin){1.0f, 1.0f, 0.35f, 10.0f, 1, {1.0f, 1.0f}},
		(t_perlin){1.8f, 0.35f, 0.02f, 1.0f, 5, {100.0f, TAU / 200.0f}},
		(t_perlin){1.8f, 0.35f, 0.02f, 1.0f, 5, {1.0f, 1.0f}},
	};
	int			i;

	i = 0;
	while (i < 4)
	{
		if (types[i] == *type)
		{
			*fp = fp_perl[i];
			override_unset_perlin_values(dst, p_data + i);
			*type = TEX_PERLIN;
			return ;
		}
		++i;
	}
	*fp = pink_noise;
	override_unset_perlin_values(dst, p_data);
	*type = TEX_PERLIN;
}

static void	set_texture_pattern(t_mat *mat)
{
	if (mat->texture.type == TEX_CHECKER)
		mat->get_texcol = checker_pattern;
	else if (mat->texture.type == TEX_IMAGE)
		mat->get_texcol = image_pattern;
	else if ((mat->texture.type & TEX_IS_PERLIN) != 0)
	{
		set_perlin_pattern(&mat->texture.type, &mat->texture.p_data,
			&mat->texture.fp_perlin);
		mat->get_texcol = noise_pattern;
	}
	else
		mat->get_texcol = solid_pattern;
	if ((mat->bump_map.type & TEX_IS_PERLIN) != 0)
	{
		set_perlin_pattern(&mat->bump_map.type, &mat->bump_map.p_data,
			&mat->bump_map.fp_perlin);
	}
	else if (mat->bump_map.type != TEX_IMAGE)
		mat->bump_map.type = TEX_SOLID;
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
	t_field	fields[30];

	count = 0;
	mat->type = type;
	init_material_fields(fields, &count, mat);
	init_bump_fields(fields, &count, mat);
	init_texture_fields(fields, &count, mat);
	init_perlin_fields(fields, &count, &mat->texture.p_data);
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
