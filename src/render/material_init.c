/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   material_init.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/31 23:53:11 by bewong        #+#    #+#                 */
/*   Updated: 2025/07/27 20:57:15 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "scene.h"
#include "rt_math.h"

#define DFLT_MAT_COUNT 3

static t_mat	*create_base_material(const char *name)
{
	t_mat	*mat;

	mat = ft_calloc(1, sizeof(t_mat));
	if (!mat)
		return (NULL);
	mat->name = ft_strdup(name);
	if (!mat->name)
		return (free(mat), NULL);
	mat->albedo = g_v3f_one;
	mat->bump_map.scale = 1.0f;
	return (mat);
}

static void	init_texture(t_mat *mat)
{
	mat->texture.type = TEX_SOLID;
	mat->texture.scale_rot = init_v3f(1.0f, 1.0f, 0.0f);
	mat->texture.col = g_v3f_one;
	mat->texture.tex = NULL;
	mat->get_texcol = solid_pattern;
}

t_mat	*init_material(t_mat_type type, const char *name)
{
	t_mat	*mat;

	mat = create_base_material(name);
	if (!mat)
		return (NULL);
	mat->type = type;
	init_texture(mat);
	if (type == MAT_LAMBERTIAN)
	{
		mat->lamb.specular = 0.8f;
		mat->lamb.shininess = 32.0f;
		mat->lamb.roughness = 0.0f;
	}
	else if (type == MAT_METAL)
		mat->metal.roughness = 0.1f;
	else if (type == MAT_DIELECTRIC)
	{
		mat->diel.ir = 1.5f;
		mat->diel.transmittance = 1.0f;
		mat->diel.roughness = 0.0f;
	}
	return (mat);
}

bool	create_default_materials(t_vector *shared_materials)
{
	const char *const	default_names[DFLT_MAT_COUNT] = {"m_default",
		"m_metal", "m_dielectric"
	};
	const t_mat_type	types[DFLT_MAT_COUNT] = {MAT_LAMBERTIAN,
		MAT_METAL, MAT_DIELECTRIC
	};
	t_mat				*mat;
	int					i;

	i = 0;
	while (i < DFLT_MAT_COUNT)
	{
		mat = init_material(types[i], default_names[i]);
		if (mat == NULL || !vector_add(shared_materials, mat))
			return (free(mat), false);
		++i;
	}
	return (true);
}
