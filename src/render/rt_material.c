/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_material.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/31 19:11:17 by bewong        #+#    #+#                 */
/*   Updated: 2025/06/12 15:34:28 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "scene.h"
#include "material.h"
#include "light.h"
#include "minirt.h"
#include "rt_math.h"
#include "random.h"
#include <stdlib.h>

static float	get_refraction_ratio(t_ray_hit *hit)
{
	if (hit->front_face && hit->obj->r.mat->diel.ir != 0.0f)
		return (1.0f / hit->obj->r.mat->diel.ir);
	return (hit->obj->r.mat->diel.ir);
}

/* Reference Links:
Physically Based Rendering (PBR) Theory:
https://learnopengl.com/PBR/Theory
Schlick's Approximation for Fresnel:
https://en.wikipedia.org/wiki/Schlick%27s_approximation
Cook-Torrance BRDF:
https://en.wikipedia.org/wiki/Specular_highlight#Cook%E2%80%93Torrance
Importance Sampling for Monte Carlo Integration:
https://www.pbr-book.org/3ed-2018/Monte_Carlo_Integration/2D_Sampling_with_Multidimensional_Transformations */

static t_v3f	blend_color(t_scene *sc, t_ray_hit *h, uint32_t depth, float ior)
{
	t_ray	ray;
	t_v3f	reflect_dir;
	t_v3f	reflect_color;
	t_v3f	refract_color;
	float	fresnel;
	t_v3f	refl;

	ray.direction = h->ray->direction;
	reflect_dir = v3f_scale(ray.direction, -1.0f);
	ior = get_refraction_ratio(h);
	ray.origin = v3f_add(h->hit, v3f_scale(h->normal, 
		h->front_face ? 0.001f : -0.001f));
	fresnel = fminf(1.0f, v3f_dot(reflect_dir, h->normal));
	fresnel = powf(schlick(fresnel, ior), 0.5f);
	refl = v3f_refl(ray.direction, h->normal);
	if (h->obj->r.mat->diel.roughness > 0.0f)
		refl = v3f_norm(v3f_add(refl, v3f_scale(random_in_hemisphere(h->normal), h->obj->r.mat->diel.roughness)));
	reflect_color = trace(&(t_ray){ray.origin, refl}, sc, depth - 1);
	refract_color = g_v3f_zero;
	if (ior * sqrtf(1.0f - powf(v3f_dot(reflect_dir, h->normal), 2)) <= 1.0f)
		refract_color = trace(&(t_ray){ray.origin, v3f_refr(ray.direction, h->normal, ior)}, sc, depth - 1);
	refract_color = v3f_mul(refract_color, h->obj->r.mat->albedo);
	return v3f_lerp(refract_color, reflect_color, fresnel);
}


// Handle dielectric material with proper transparency and refraction
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
		direct_light = v3f_mul(hit->obj->r.mat->albedo, compute_lighting(hit, sc));
	if (hit->obj->r.mat->diel.transmittance >= 0.99f)
		return (v3f_clampf01(indirect));
	if (hit->obj->r.mat->diel.transmittance <= 0.01f)
		return (v3f_clampf01(direct_light));
	result = v3f_add(
		v3f_scale(direct_light, 1.0f - hit->obj->r.mat->diel.transmittance),
		v3f_scale(indirect, hit->obj->r.mat->diel.transmittance)
	);
	return v3f_clampf01(result);
}


t_v3f	handle_lambertian(t_scene *scene, t_ray_hit *hit_info)
{
	t_v3f	base_color;
	t_v3f	lighting;

	if (!hit_info->obj->r.mat || hit_info->obj->r.mat->type != MAT_LAMBERTIAN)
		return (g_v3f_zero);
	base_color = v3f_mul(hit_info->obj->r.color, hit_info->obj->r.mat->albedo);
	lighting = compute_lighting(hit_info, scene);
	return (v3f_mul(base_color, lighting));
}

t_v3f	handle_metal(t_scene *sc, t_ray_hit *hit, uint32_t depth)
{
	t_ray	reflected_ray;
	t_v3f	reflected_dir;
	t_v3f	result;
	t_v3f	albedo;
	float	roughness;

	if (hit->obj->r.mat && hit->obj->r.mat->type == MAT_METAL)
	{
		albedo = hit->obj->r.mat->albedo;
		roughness = hit->obj->r.mat->metal.roughness;
	}
	else
	{
		albedo = hit->obj->r.color;
		roughness = 0.05f;
	}
	reflected_dir = v3f_refl(v3f_norm(hit->ray->direction), hit->normal);
	if (roughness > 0.0f)
		reflected_dir = v3f_norm(v3f_add(reflected_dir, v3f_scale(random_in_hemisphere(hit->normal), roughness)));
	reflected_ray.origin = v3f_add(hit->hit, v3f_scale(hit->normal, 0.001f));
	reflected_ray.direction = reflected_dir;
	if (v3f_dot(reflected_ray.direction, hit->normal) > 0.0f)
	{
		result = trace(&reflected_ray, sc, depth - 1);
		return v3f_mul(albedo, result);
	}
	return g_v3f_zero;
}

