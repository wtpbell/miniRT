/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   material_utils.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/06 19:17:23 by jboon         #+#    #+#                 */
/*   Updated: 2025/06/07 21:31:09 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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

bool	assign_material(t_obj *obj, t_vector *materials, const char *m_name)
{
	if (m_name == NULL)
		m_name = "m_default";
	if (!is_valid_material_name(m_name))
		return (print_error(ERR_INV_MAT_NAME, "material", m_name), false);
	obj->r.mat = find_or_create_material(materials, m_name);
	return (obj->r.mat != NULL);
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
