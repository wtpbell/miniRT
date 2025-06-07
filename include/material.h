/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   material.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/29 13:47:23 by bewong        #+#    #+#                 */
/*   Updated: 2025/06/07 16:39:46 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATERIAL_H
# define MATERIAL_H

# include "rt_types.h"
# include "scene.h"

enum e_material_type
{
	MAT_UNKNOWN,
	MAT_LAMBERTIAN,
	MAT_METAL,
	MAT_DIELECTRIC
};

struct s_lambertian
{
	float	specular;
	float	shininess;
};

struct s_metal
{
	float	fuzz;
};

struct s_dielectric
{
	float	ir;
	float	transmittance;
};

struct s_material
{
	char		*name;
	t_mat_type	type;
	t_v3f		albedo;
	union
	{
		t_lamb	lamb;
		t_metal	metal;
		t_diel	diel;
	};
};

t_mat	*init_material(t_mat_type type, const char *name);
bool	create_default_materials(t_vector *shared_materials);
t_mat	*find_or_create_material(t_vector *materials, const char *m_name);
bool	assign_material(t_obj *obj, t_vector *materials, const char *m_name);

#endif
