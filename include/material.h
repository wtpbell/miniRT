/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 13:47:23 by bewong            #+#    #+#             */
/*   Updated: 2025/06/10 19:45:31 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATERIAL_H
# define MATERIAL_H

# include "rt_types.h"

enum e_material_type
{
	MAT_UNKNOWN,
	MAT_LAMBERTIAN,
	MAT_METAL,
	MAT_DIELECTRIC
};

struct s_lambertian
{
	float	specular;   // 0.0 = no specular, 1.0 = full specular
	float	shininess;  // Higher values = tighter specular highlight
	float	roughness;  // 0.0 = smooth, 1.0 = rough
};

typedef struct s_lambertian	t_lamb;

struct s_metal
{
	float	fuzz;       // 0.0 = perfect mirror, 1.0 = very fuzzy
	float	roughness;  // 0.0 = smooth, 1.0 = rough
};

typedef struct s_metal	t_metal;

struct s_dielectric
{
	float	ir;            // Index of refraction
	float	transmittance; // 0.0 = opaque, 1.0 = fully transparent
	float	roughness;     // 0.0 = smooth, 1.0 = rough
};

typedef struct s_dielectric	t_diel;

struct s_material
{
	char		*name;
	t_mat_type	type;
	t_v3f		albedo;  // Base color
	union
	{
		t_lamb	lamb;    // Lambertian
		t_metal	metal;   // Metal
		t_diel	diel;    // Dielectric
	};
};

t_mat	*init_material(t_mat_type type, const char *name);
bool	create_default_materials(t_vector *shared_materials);
bool	assign_material(t_obj *obj, t_vector *materials, const char *m_name);
t_v3f	handle_dielectric(t_scene *sc, t_ray_hit *hit, uint32_t depth);
t_v3f	handle_lambertian(t_scene *scene, t_ray_hit *hit_info);
t_v3f	handle_metal(t_scene *sc, t_ray_hit *hit, uint32_t depth);

#endif
