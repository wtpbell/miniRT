/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   material.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/05 14:04:02 by jboon         #+#    #+#                 */
/*   Updated: 2025/06/05 19:31:00 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

#define M_COUNT 3

typedef enum e_field_type
{
	FIELD_INT,
	FIELD_FLOAT,
	FIELD_V3F
}	t_f_type;

typedef struct s_field_material
{
	const char	*prefix;
	void		*val;
	t_f_type	field_type;
	t_v2f		limit;
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

t_mat	*find_material(t_vector *materials, const char *m_name)
{
	int	i;

	i = 0;
	while (i < materials->size)
	{
		if (ft_strcmp(((t_mat *)materials->items[i])->name, m_name) == 0)
			return ((t_mat *)materials->items[i]);
		++i;
	}
	return (NULL);
}

int	is_valid_name(const char *m_name)
{
	if (ft_strncmp(m_name, "m_", 2) != 0)
		return (0);
	m_name += 2;
	while (ft_isalnum(*m_name) || (*m_name == '_' && *(m_name - 1) != '_'))
		++m_name;
	return (m_name == '\0');
}

t_mat_type	get_mat_type(const char *token)
{
	const char	*sep;

	sep = ft_strchr(token, ':');
	if (sep == NULL || ft_strncmp("type", token, (sep - token)) != 0)
		return (MAT_UNKNOWN); // error message?
	++sep;
	if (ft_strcmp("lambertian", sep) == 0)
		return (MAT_LAMBERTIAN);
	else if (ft_strcmp("metal", sep) == 0)
		return (MAT_METAL);
	else if (ft_strcmp("dielectric", sep) == 0)
		return (MAT_DIELECTRIC);
	return (MAT_UNKNOWN);
}

int	parse_ffield(const char *token, const char *field, float *val, t_v2f range)
{
	const char	*sep;

	sep = ft_strchr(token, ':');
	if (sep == NULL || *(sep + 1) == '\0'
		|| ft_strncmp(field, token, (sep - token) != 0))
		return (0);
	return (parse_and_validate_float(val, sep + 1, range, sep));
}

int	parse_v3ffield(const char *token, const char *field, t_v3f *v)
{

}

int	parse_fields(t_f_mat *fields, int size, const char **tokens)
{
	int	i;

	while (i < size)
	{
		if (fields[i].field_type == FIELD_FLOAT && parse_and_validate_float(fields[i].val, NULL, fields[i].limit, NULL))
	}
}

int	parse_lambertian(t_mat *mat)
{
	const t_f_mat	fields[2] = {
		{"spc", &mat->lamb.specular, FIELD_FLOAT, init_v2f(0.0f, 1.0f)},
		{"shi", &mat->lamb.shininess, FIELD_FLOAT, init_v2f(0.0, 1.0f)}
	};


}

int	parse_material(const char **tokens, t_scene *scene)
{
	t_mat		*mat;
	const char	*m_name;
	t_mat_type	mat_type;

	if (is_valid_name(tokens[0]))
		return (0);
	m_name = tokens[0];
	mat_type = get_mat_type(tokens[1]);
	if (mat_type == MAT_UNKNOWN)
		return (0);
	if (mat_type == MAT_LAMBERTIAN)




	// is_valid_name
	// handle type
	// overwrite or create new

	mat = find_material(&scene->shared_materials, tokens[0]);
}
