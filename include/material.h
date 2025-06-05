/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   material.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/29 13:47:23 by bewong        #+#    #+#                 */
/*   Updated: 2025/06/05 16:32:52 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATERIAL_H
# define MATERIAL_H

# include "scene.h"

typedef enum e_material_type
{
	MAT_UNKNOWN,
	MAT_LAMBERTIAN,
	MAT_METAL,
	MAT_DIELECTRIC
}	t_mat_type;

typedef struct s_lambertian
{
	float	specular;
	float	shininess;
}	t_lamb;

typedef struct s_metal
{
	float	fuzz;
}	t_metal;

typedef struct s_dielectric
{
	float	ir;
	float	transmittance;
}	t_diel;

typedef struct s_material
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
}	t_mat;

t_mat	*init_material(t_mat_type type, const char *name);
t_mat	*create_lambertian(t_v3f albedo, float specular, float shininess);
t_mat	*create_metal(t_v3f albedo, float fuzz);
t_mat	*create_dielectric(t_v3f albedo, float ir, float transmittance);

#endif
