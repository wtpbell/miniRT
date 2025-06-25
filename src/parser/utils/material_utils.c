/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 19:17:23 by jboon             #+#    #+#             */
/*   Updated: 2025/06/25 20:00:26 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>  // For PATH_MAX
#include <dirent.h>  // For directory operations
#include <fcntl.h>   // For open()
#include <stdio.h>   // For printf and fprintf
#include <stdlib.h>  // For free, malloc
#include <string.h>  // For strdup, strndup, strlen
#include "parser.h"
#include "material.h"
#include "rt_math.h"

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


bool	load_bump_map(t_mat *mat, const char *bump_path)
{
	mlx_texture_t	*bump_map;
	
	if (!mat || !bump_path || !*bump_path)	
		return (print_error(ERR_INV_MAT_NAME, "material", bump_path), false);
	bump_map = load_png_texture(bump_path);
	if (!bump_map)
		return (print_error(ERR_LOAD_TEXTURE, "material", bump_path), false);
	if (mat->bump_map)
		mlx_delete_texture(mat->bump_map);
	mat->bump_map = bump_map;
	if (mat->bump_scale <= 0.0f)
		mat->bump_scale = 0.1f;
	return (true);
}

bool	assign_material(t_obj *obj, t_vector *materials, const char *m_name)
{
	t_mat	*mat;

	if (!m_name || !*m_name)
		m_name = "m_default";
	if (!is_valid_material_name(m_name))
		return (print_error(ERR_INV_MAT_NAME, "material", m_name), false);
	mat = find_or_create_material(materials, m_name);
	if (!mat)
		return (print_error(ERR_MEM, "material", NULL), false);
	if (mat->tex_path && !load_texture(&mat->texture, mat->tex_path))
		return (print_error(ERR_LOAD_TEXTURE, "material", mat->tex_path), false);
	if (mat->bump_path && !load_bump_map(mat, mat->bump_path))
		return (print_error(ERR_LOAD_TEXTURE, "material", mat->bump_path), false);
	obj->r.mat = mat;
	return (true);
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
