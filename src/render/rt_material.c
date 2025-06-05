/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_material.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/02 11:16:23 by bewong        #+#    #+#                 */
/*   Updated: 2025/06/05 11:35:23 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "scene.h"
#include "material.h"
#include "light.h"
#include "minirt.h"
#include "rt_math.h"
#include "random.h"
#include <stdlib.h>

static float	get_refraction_ratio(t_ray_hit *hit_info)
{
	if (hit_info->front_face)
		return (1.0f / hit_info->obj->r.mat->diel.ir);
	return (hit_info->obj->r.mat->diel.ir);
}

static t_v3f	blend_color(t_scene *sc, t_ray_hit *hit,
				uint32_t depth, float ior)
{
	t_ray	ray;
	t_v3f	colors[2];
	float	cos_theta;
	float	reflectance;

	ray.direction = v3f_norm(hit->ray->direction);
	if (hit->front_face && hit->obj->r.mat->diel.ir != 0.0f)
		ior = 1.0f / hit->obj->r.mat->diel.ir;
	else
		ior = hit->obj->r.mat->diel.ir;
	cos_theta = fminf(1.0f, v3f_dot(v3f_scale(ray.direction, -1.0f), hit->normal));
	reflectance = schlick(cos_theta, ior);
	if (hit->front_face)
		ray.origin = v3f_add(hit->hit, v3f_scale(hit->normal, 0.001f));
	else
		ray.origin = v3f_add(hit->hit, v3f_scale(hit->normal, -0.001f)); // inside the obj
	ray.direction = v3f_refl(ray.direction, hit->normal);
	colors[0] = trace(&ray, sc, depth - 1); // trace reflection ray
	if (ior * sqrtf(1.0f - cos_theta * cos_theta) <= 1.0f) // if total internal reflection
	{
		ray.direction = v3f_refr(ray.direction, hit->normal, ior);
		colors[1] = trace(&ray, sc, depth - 1); // trace refraction ray
	}
	else
		colors[1] = g_v3f_zero; // no refraction
	return (v3f_lerp(colors[1], colors[0], reflectance)); // blend reflection and refraction
}

// 0.1f more perfect, 0.3f more visible diffuse lighting, 0.0f pure transparent
t_v3f	handle_dielectric(t_scene *sc, t_ray_hit *hit, uint32_t depth)
{
	t_v3f	direct;
	t_v3f	indirect;
	float	kd;

	direct = handle_lambertian(sc, hit);
	indirect = blend_color(sc, hit, depth, get_refraction_ratio(hit));
	kd = 1.0f - hit->obj->r.mat->diel.transmittance - BIAS;  // Diffuse weight = 1.0 - transmittance - 0.2f(small bias) bias is removed
	return (v3f_mul(
			v3f_add(v3f_scale(direct, kd), v3f_scale(indirect, (1.0f - kd)))
			, hit->obj->r.mat->albedo));
}

t_v3f	handle_lambertian(t_scene *scene, t_ray_hit *hit_info)
{
	t_light	*light;
	t_v3f	total_light;
	t_v3f	obj_albedo;
	int		i;

	total_light = g_v3f_zero;
	obj_albedo = hit_info->obj->r.mat->albedo;
	i = 0;
	while (i < scene->lights.size)
	{
		light = (t_light *)scene->lights.items[i];
		if (light->type == LIGHT_POINT)
			total_light = v3f_add(total_light, apply_point(scene, hit_info, light));
		else if (light->type == LIGHT_AMBIENT)
			total_light = v3f_add(total_light, apply_ambient(obj_albedo, light));
		i++;
	}
	return (v3f_mul(obj_albedo, total_light));
}

t_v3f	handle_metal(t_scene *sc, t_ray_hit *hit, uint32_t depth)
{
	t_ray	reflected_ray;
	t_v3f	reflected_dir;
	t_v3f	random_fuzz;
	t_metal	metal_data;
	t_v3f	result;

	metal_data = hit->obj->r.mat->metal;
	reflected_dir = v3f_refl(v3f_norm(hit->ray->direction), hit->normal);
	if (metal_data.fuzz > 0.0f) //random only when perfect mirror
	{
		random_fuzz = v3f_scale(random_in_hemisphere(hit->normal), metal_data.fuzz);
		// random_fuzz = v3f_scale(random_direction(), metal_data.fuzz);
		reflected_dir = v3f_add(reflected_dir, random_fuzz);
	}
	if (v3f_dot(reflected_dir, hit->normal) <= 0.0f)
		return (g_v3f_zero); //discard invalid bounce
	reflected_ray.origin = v3f_add(hit->hit, v3f_scale(hit->normal, BIAS));
	reflected_ray.direction = v3f_norm(reflected_dir);
	result = trace(&reflected_ray, sc, depth - 1);
	return (v3f_mul(hit->obj->r.mat->albedo, result));
	// return (result);
}

