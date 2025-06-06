/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   material.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/05 14:04:02 by jboon         #+#    #+#                 */
/*   Updated: 2025/06/06 16:53:00 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

#define M_COUNT 3

typedef enum e_field_type
{
	FIELD_INT,
	FIELD_FLOAT,
	FIELD_V3F,
	FIELD_COL
}	t_f_type;

typedef enum e_field_state
{
	EMPTY		= 0x0,
	FILLED		= 0x1,
	REQUIRED	= 0x2
}	t_f_state;

typedef struct s_field_material
{
	const char	*prefix;
	void		*val;
	t_f_type	field_type;
	t_v2f		limit;
	t_f_state	state;
}	t_f_mat;


int	create_default_materials(t_scene *scene)
{
	const char			*default_names[M_COUNT] = {"m_default", "m_metal", "m_dielectric"};
	const t_mat_type	types[M_COUNT] = {MAT_LAMBERTIAN, MAT_METAL, MAT_DIELECTRIC};
	t_vector			materials;
	t_mat				*mat;
	int					i;

	if (!vector_init(&materials, 8))
		return (0);
	i = 0;
	while (i < M_COUNT)
	{
		mat = init_material(types[i], default_names[i]);
		if (mat == NULL || !vector_add(&materials, mat))
			return (free(mat), vector_free(&materials, free), 0);
		++i;
	}
	scene->shared_materials = materials;
	return (1);
}

t_mat	*find_or_create_material(t_vector *materials, const char *m_name)
{
	int		i;
	t_mat	*mat;

	i = 0;
	while (i < materials->size)
	{
		if (ft_strcmp(((t_mat *)materials->items[i])->name, m_name) == 0)
			return ((t_mat *)materials->items[i]);
		++i;
	}
	mat = init_material(MAT_LAMBERTIAN, m_name);
	if (mat == NULL || !vector_add(materials, mat))
		return (free_material(mat), NULL);
	return (mat);
}

int	is_valid_material_name(const char *m_name)
{
	if (ft_strncmp(m_name, "m_", 2) != 0 || *(m_name + 2) == '\0')
		return (0);
	m_name += 2;
	while (ft_isalnum(*m_name) || (*m_name == '_' && *(m_name - 1) != '_'))
		++m_name;
	return (*m_name == '\0');
}

inline t_f_mat	init_fmat(const char *name, void *mem, t_f_type type, t_v2f lim)
{
	return (t_f_mat){name, mem, type, lim, EMPTY | REQUIRED};
}

int	parse_fields(t_f_mat *fields, int field_count, char **tokens)
{
	const char	*value;
	int			i;

	value = NULL;
	while (*tokens)
	{
		i = 0;
		while (i < field_count)
		{
			value = ft_strchr(*tokens, ':');
			if (value != NULL
				&& ft_strncmp(fields[i].prefix, *tokens, (value - *tokens)) == 0)
				break ;
			++i;
		}
		if (i == field_count)
			return (print_error(ERR_UNKNOWN_FIELD, "material", *tokens), 0);
		if ((fields[i].field_type == FIELD_FLOAT && !parse_and_validate_float(fields[i].val, value + 1, fields[i].limit, *tokens))
			|| (fields[i].field_type == FIELD_INT && !parse_and_validate_int(fields[i].val, value + 1, fields[i].limit, *tokens))
			|| (fields[i].field_type == FIELD_V3F && !parse_v3f(fields[i].val, value + 1))
			|| (fields[i].field_type == FIELD_COL && !parse_col(fields[i].val, value + 1)))
			return (0); // Any errors are printed out by the parsers
		fields[i].state |= FILLED;
		++tokens;
	}
	i = 0;
	while (i < field_count)
	{
		if ((fields[i].state & REQUIRED) == REQUIRED
			&& (fields[i].state & FILLED) == EMPTY)
			return (print_error(ERR_REQ_FIELD, "material", fields[i].prefix), 0);
		++i;
	}
	return (1);
}

int	parse_lambertian(t_mat *mat, char **tokens)
{
	t_f_mat	fields[3];

	mat->type = MAT_LAMBERTIAN;
	fields[0] = init_fmat("spc", &mat->lamb.specular, FIELD_FLOAT, init_v2f(0.0f, 1.0f));
	fields[1] = init_fmat("shi", &mat->lamb.shininess, FIELD_FLOAT, init_v2f(0.0, 1.0f));
	fields[2] = init_fmat("alb", &mat->albedo, FIELD_COL, init_v2f(0.0, 1.0f));
	fields[2].state ^= REQUIRED;
	return (parse_fields(fields, 3, tokens));
}

int	parse_metal(t_mat *mat, char **tokens)
{
	t_f_mat fields[2];

	mat->type = MAT_METAL;
	fields[0] = init_fmat("fuz", &mat->metal.fuzz, FIELD_FLOAT, init_v2f(0.0f, 1.0f));
	fields[1] = init_fmat("alb", &mat->albedo, FIELD_COL, init_v2f(0.0, 1.0f));
	fields[1].state ^= REQUIRED;
	return (parse_fields(fields, 2, tokens));
}

int	parse_dielectric(t_mat *mat, char **tokens)
{
	t_f_mat	fields[3];

	mat->type = MAT_DIELECTRIC;
	fields[0] = init_fmat("ir", &mat->lamb.specular, FIELD_FLOAT, init_v2f(0.0f, 1.0f));
	fields[1] = init_fmat("trs", &mat->lamb.shininess, FIELD_FLOAT, init_v2f(0.0, 1.0f));
	fields[2] = init_fmat("alb", &mat->albedo, FIELD_COL, init_v2f(0.0, 1.0f));
	fields[2].state ^= REQUIRED;
	return (parse_fields(fields, 3, tokens));
}

t_mat_type	get_mat_type(const char *value)
{
	if (ft_strcmp("lambertian", value) == 0)
		return (MAT_LAMBERTIAN);
	else if (ft_strcmp("metal", value) == 0)
		return (MAT_METAL);
	else if (ft_strcmp("dielectric", value) == 0)
		return (MAT_DIELECTRIC);
	return (MAT_UNKNOWN);
}

bool	check_type_field(const char *token, const char **value)
{
	*value = ft_strchr(token, ':');
	if (*value == NULL || ft_strncmp("type", token, (*value - token)) != 0)
		return (false);
	*value += 1;
	return (true);
}

bool	parse_material(char **tokens, t_scene *scene)
{
	t_mat		*mat;
	t_mat_type	mat_type;
	const char	*str_type;

	str_type = NULL;
	if (!is_valid_material_name(tokens[0]))
		return (print_error(ERR_INV_MAT_NAME, "material" , tokens[0]), false);
	if (tokens[1] == NULL || !check_type_field(tokens[1], &str_type))
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

bool	assign_material(t_obj *obj, t_vector *materials, const char *m_name)
{
	if (m_name == NULL)
		m_name = "m_default";
	if (!is_valid_material_name(m_name))
		return (print_error(ERR_INV_MAT_NAME, "material" , m_name), false);
	obj->r.mat = find_or_create_material(materials, m_name);
	return (obj->r.mat != NULL);
}
