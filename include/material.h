/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 13:47:23 by bewong            #+#    #+#             */
/*   Updated: 2025/06/13 18:07:29 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATERIAL_H
# define MATERIAL_H

# include <stdint.h>
# include "rt_types.h"
# include "minirt.h"
# include "container.h"

enum e_material_type
{
	MAT_UNKNOWN,
	MAT_LAMBERTIAN,
	MAT_METAL,
	MAT_DIELECTRIC
};

struct s_material
{
	char		*name;
	t_mat_type	type;
	t_v3f		albedo;
	struct {
		float	specular;
		float	shininess;
		float	roughness;
	}	lamb;
	struct {
		float	roughness;
	}	metal;
	struct {
		float	ir;
		float	transmittance;
		float	roughness;
	}	diel;
};

t_mat	*init_material(t_mat_type type, const char *name);
bool	create_default_materials(t_vector *shared_materials);
bool	assign_material(t_obj *obj, t_vector *materials, const char *m_name);
// float	get_refraction_ratio(t_ray_hit *hit);
// t_v3f	get_reflect_color(t_scene *sc, t_ray_hit *h, uint32_t depth);
// t_v3f	calculate_refr(t_scene *sc, t_ray_hit *h, float ior, uint32_t depth);
t_v3f	handle_dielectric(t_scene *sc, t_ray_hit *hit, uint32_t depth);
t_v3f	handle_lambertian(t_scene *scene, t_ray_hit *hit_info);
t_v3f	handle_metal(t_scene *sc, t_ray_hit *hit, uint32_t depth);

#endif
