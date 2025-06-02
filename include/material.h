/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   material.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/29 13:47:23 by bewong        #+#    #+#                 */
/*   Updated: 2025/06/02 11:43:51 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATERIAL_H
# define MATERIAL_H

# include "vector.h"
# include "ray.h"

typedef struct s_scene t_scene;

typedef enum e_material_type
{
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
	t_mat_type	type;
	t_v3f		albedo;
	union
	{
		t_lamb	lamb;
		t_metal	metal;
		t_diel	diel;
	};
}	t_mat;

t_mat	*init_material(t_mat_type type);
t_mat	*create_lambertian(t_v3f albedo, float specular, float shininess);
t_mat	*create_metal(t_v3f albedo, float fuzz);
t_mat	*create_dielectric(t_v3f albedo, float ir, float transmittance);
t_v3f	handle_dielectric(t_scene *sc, t_ray_hit *hit, uint32_t depth);
t_v3f	handle_lambertian(t_scene *scene, t_ray_hit *hit_info);

#endif
