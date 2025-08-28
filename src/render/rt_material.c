/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_material.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/02 11:16:23 by bewong        #+#    #+#                 */
/*   Updated: 2025/08/28 17:14:27 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "light.h"
#include "material.h"
#include "minirt.h"
#include "random.h"
#include "ray.h"
#include "rt_math.h"
#include "scene.h"

t_v3f	handle_dielectric(t_scene *sc, t_ray_hit *hit, uint32_t depth)
{
	t_v3f	direct_light;
	t_v3f	indirect;
	t_v3f	result;
	float	ior;

	direct_light = g_v3f_zero;
	indirect = g_v3f_zero;
	if (hit->front_face)
		ior = 1.0f / hit->obj->r.mat->diel.ir;
	else
		ior = hit->obj->r.mat->diel.ir;
	indirect = blend_color(sc, hit, depth, ior);
	if (hit->obj->r.mat->diel.transmittance < 0.99f)
		direct_light = v3f_mul(hit->hit_color, compute_lighting(hit, sc));
	if (hit->obj->r.mat->diel.transmittance >= 0.99f)
		return (v3f_clampf01(indirect));
	if (hit->obj->r.mat->diel.transmittance <= 0.01f)
		return (v3f_clampf01(direct_light));
	result = v3f_add(
			v3f_scale(direct_light, 1.0f - hit->obj->r.mat->diel.transmittance),
			v3f_scale(indirect, hit->obj->r.mat->diel.transmittance)
			);
	return (v3f_clampf01(result));
}

t_v3f	handle_lambertian(t_scene *scene, t_ray_hit *hit_info)
{
	t_v3f	lighting;
	t_v3f	org_norm;

	org_norm = hit_info->normal;
	if (hit_info->obj->r.mat->lamb.roughness > 0.0f)
	{
		hit_info->normal = v3f_norm(v3f_add(hit_info->normal, v3f_scale(
						random_in_hemisphere(hit_info->normal),
						hit_info->obj->r.mat->lamb.roughness
						)));
	}
	lighting = compute_lighting(hit_info, scene);
	hit_info->normal = org_norm;
	return (v3f_clampf01(v3f_mul(hit_info->hit_color, lighting)));
}

t_v3f	handle_metal(t_scene *sc, t_ray_hit *hit, uint32_t depth)
{
	t_ray	reflected_ray;
	t_v3f	reflected_dir;
	t_v3f	result;

	reflected_dir = v3f_refl(v3f_norm(hit->ray->direction), hit->normal);
	if (hit->obj->r.mat->metal.roughness > 0.0f)
		reflected_dir = v3f_norm(v3f_add(reflected_dir,
					v3f_scale(random_in_hemisphere(hit->normal),
						hit->obj->r.mat->metal.roughness)));
	reflected_ray.origin = v3f_add(hit->hit, v3f_scale(hit->normal, 0.001f));
	reflected_ray.direction = reflected_dir;
	if (v3f_dot(reflected_ray.direction, hit->normal) > 0.0f)
	{
		result = trace(&reflected_ray, sc, depth - 1);
		return (v3f_mul(hit->hit_color, result));
	}
	return (g_v3f_zero);
}
