/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   material_init.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/31 23:53:11 by bewong        #+#    #+#                 */
/*   Updated: 2025/06/07 17:20:39 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "scene.h"
#include "rt_math.h"

#define DFLT_MAT_COUNT 3

t_mat	*init_material(t_mat_type type, const char *name)
{
	t_mat	*mat;

	mat = ft_calloc(1, sizeof(t_mat));
	if (!mat)
		return (NULL);
	mat->name = ft_strdup(name);
	if (mat->name == NULL)
		return (free(mat), NULL);
	mat->type = type;
	mat->albedo = init_v3f(1.0f, 1.0f, 1.0f);
	if (type == MAT_LAMBERTIAN)
	{
		mat->lamb.specular = 0.7f;
		mat->lamb.shininess = 64.0f;
	}
	else if (type == MAT_METAL)
		mat->metal.fuzz = 0.1f;
	else if (type == MAT_DIELECTRIC)
	{
		mat->diel.ir = 1.5f;
		mat->diel.transmittance = 0.9f;
	}
	return (mat);
}

bool	create_default_materials(t_vector *shared_materials)
{
	const char * const	default_names[DFLT_MAT_COUNT] = {"m_default",
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
