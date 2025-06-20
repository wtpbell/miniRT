/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_material.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/02 11:16:23 by bewong        #+#    #+#                 */
/*   Updated: 2025/06/18 17:46:01 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "light.h"
#include "material.h"
#include "minirt.h"
#include "random.h"
#include "ray.h"
#include "rt_math.h"
#include "scene.h"

static float	get_refraction_ratio(t_ray_hit *hit)
{
	if (hit->front_face && hit->obj->r.mat->diel.ir != 0.0f)
		return (1.0f / hit->obj->r.mat->diel.ir);
	return (hit->obj->r.mat->diel.ir);
}

static t_v3f	blend_color(t_scene *sc, t_ray_hit *h,
						uint32_t depth, float ior)
{
	t_ray	ray;
	t_v3f	reflect_dir;
	t_v3f	reflect_color;
	t_v3f	refract_color;
	float	fresnel;

	ray.direction = h->ray->direction;
	reflect_dir = v3f_scale(ray.direction, -1.0f);
	if (h->front_face)
		ray.origin = v3f_add(h->hit, v3f_scale(h->normal, 0.001f));
	else
		ray.origin = v3f_add(h->hit, v3f_scale(h->normal, -0.001f));
	fresnel = schlick(fminf(1.0f, v3f_dot(reflect_dir, h->normal)), ior);
	fresnel = powf(fresnel, 0.5f);
	reflect_dir = v3f_refl(ray.direction, h->normal);
	if (h->obj->r.mat->diel.roughness > 0.0f)
		reflect_dir = v3f_norm(v3f_add(reflect_dir, v3f_scale(random_in_hemisphere(h->normal),
			h->obj->r.mat->diel.roughness)));
	reflect_color = v3f_mul(trace(&(t_ray){ray.origin, reflect_dir}, sc, depth - 1),
		h->obj->r.mat->albedo);
	refract_color = g_v3f_zero;
	if (ior * sqrtf(1.0f - powf(v3f_dot(v3f_scale(ray.direction, -1.0f), h->normal), 2)) <= 1.0f)
		refract_color = v3f_mul(trace(&(t_ray){ray.origin, v3f_refr(ray.direction, h->normal, 
			get_refraction_ratio(h))}, sc, depth - 1), h->obj->r.mat->albedo);
	return (v3f_lerp(refract_color, reflect_color, fresnel));
}

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
		direct_light = v3f_mul(compute_lighting(hit, sc), hit->hit_color);
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

	lighting = compute_lighting(hit_info, scene);
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
