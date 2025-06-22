/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   light_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/13 12:17:53 by bewong        #+#    #+#                 */
/*   Updated: 2025/06/22 11:59:22 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "light.h"
#include "material.h"
#include "ray.h"
#include "rt_math.h"

static float	calculate_specular(t_lighting *lighting,
			float shininess, float specular_strength)
{
	t_v3f	refl_dir;
	float	spec;

	refl_dir = v3f_refl(v3f_scale(
				lighting->light_dir, -1.0f), lighting->normal);
	spec = powf(ft_maxf(0.0f, v3f_dot(
					lighting->view_dir, refl_dir)), shininess);
	return (spec * specular_strength);
}

float	calculate_diffuse(t_lighting *lighting)
{
	float	n_dot_l;
	float	energy;

	n_dot_l = ft_clampf01(v3f_dot(lighting->normal,
				lighting->light_dir) * 0.9f);
	energy = 1.0f - (0.3f * lighting->specular);
	return (ft_maxf(0.0f, n_dot_l * energy * 0.95f));
}

float	get_specular(t_lighting *lt, t_ray_hit *hit)
{
	if (hit->obj->r.mat->type == MAT_LAMBERTIAN
		&& hit->obj->r.mat->lamb.specular > 0.0f)
		return (calculate_specular(lt, hit->obj->r.mat->lamb.shininess,
				hit->obj->r.mat->lamb.specular));
	if (hit->obj->r.mat->type == MAT_DIELECTRIC)
		return (schlick(v3f_dot(lt->normal, lt->view_dir),
				hit->obj->r.mat->diel.ir));
	if (hit->obj->r.mat->type == MAT_METAL)
		return (calculate_specular(lt, 100.0f, 1.0f)
			* (1.0f - hit->obj->r.mat->metal.roughness));
	return (0.0f);
}
