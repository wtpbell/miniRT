/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_material.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/31 19:11:17 by bewong        #+#    #+#                 */
/*   Updated: 2025/06/11 15:52:32 by bewong        ########   odam.nl         */
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

	ray.direction = v3f_norm(h->ray->direction);
	reflect_dir = v3f_scale(ray.direction, -1.0f);
	ior = get_refraction_ratio(h);
	ray.origin = v3f_add(h->hit, v3f_scale(h->normal, 
		h->front_face * 0.001f + !h->front_face * -0.001f));
	fresnel = fminf(1.0f, v3f_dot(reflect_dir, h->normal));
	fresnel = powf(schlick(fresnel, ior), 0.5f);
	reflect_color = trace(&(t_ray){ray.origin, v3f_refl(ray.direction, h->normal)}, sc, depth - 1);
	refract_color = g_v3f_zero;
	if (ior * sqrtf(1.0f - powf(v3f_dot(reflect_dir, h->normal), 2)) <= 1.0f)
		refract_color = trace(&(t_ray){ray.origin, v3f_refr(ray.direction, h->normal, ior)}, sc, depth - 1);
	return (v3f_mul(v3f_lerp(refract_color, reflect_color, fresnel), h->obj->r.color));
}

// Handle dielectric material with proper transparency and refraction
t_v3f handle_dielectric(t_scene *sc, t_ray_hit *hit, uint32_t depth)
{
	t_v3f	direct_light;
	t_v3f	indirect;
	t_v3f	result;
	t_light	*light;
	float	ior;
	int		i;

	direct_light = g_v3f_zero;
	indirect = g_v3f_zero;
	if (hit->front_face)
		ior = 1.0f / hit->obj->r.mat->diel.ir;
	else
		ior = hit->obj->r.mat->diel.ir;
	indirect = blend_color(sc, hit, depth, ior);
	if (hit->obj->r.mat->diel.transmittance < 0.99f)
	{
		i = 0;
		while (i < sc->lights.size)
		{
			light = (t_light *)sc->lights.items[i];
			if (light->type == LIGHT_AMBIENT)
				direct_light = v3f_scale(light->color, light->intensity);
			else if (light->type == LIGHT_POINT)
				direct_light = v3f_add(direct_light, apply_point(sc, hit, light));
			i++;
		}
		direct_light = v3f_mul(hit->obj->r.color, direct_light);
	}
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
	t_light		*light;
	t_lighting	lighting;
	t_v3f		total_light;
	t_v3f		light_contribution;
	t_ren		render;
	int			i;
	float		spec;

	total_light = g_v3f_zero;
	render = hit_info->obj->r;
	if (hit_info->obj->r.mat && render.mat->type == MAT_LAMBERTIAN)
		render.color = v3f_mul(render.color, render.mat->albedo);
	i = 0;
	while (i < scene->lights.size)
	{
		light = (t_light *)scene->lights.items[i];
		if (light->type == LIGHT_POINT)
		{
			light_contribution = apply_point(scene, hit_info, light);
			init_lighting(&lighting, hit_info, light, scene->camera.t.pos);
			light_contribution = v3f_scale(light_contribution, 1.0f - (render.mat->lamb.roughness * 0.5f));
			if (hit_info->obj->r.mat->lamb.specular > 0.0f)
			{
				spec = calculate_specular(&lighting, render.mat->lamb.shininess, render.mat->lamb.specular);
				light_contribution = v3f_add(light_contribution, v3f_scale(light->color, spec * light->intensity));
			}
			
			total_light = v3f_add(total_light, light_contribution);
		}
		else if (light->type == LIGHT_AMBIENT)
			total_light = v3f_add(total_light, apply_ambient(render.color, light));
		i++;
	}
	return (v3f_mul(render.color, v3f_clamp(total_light, 0.0f, 1.0f)));
}

t_v3f	handle_metal(t_scene *sc, t_ray_hit *hit, uint32_t depth)
{
	t_ray	reflected_ray;
	t_v3f	reflected_dir;
	t_v3f	result;
	t_v3f	albedo;
	float	fuzz;

	if (hit->obj->r.mat && hit->obj->r.mat->type == MAT_METAL)
	{
		albedo = hit->obj->r.mat->albedo;
		fuzz = hit->obj->r.mat->metal.fuzz;
	}
	else
	{
		albedo = hit->obj->r.color;
		fuzz = 0.05f;
	}
	reflected_dir = v3f_refl(v3f_norm(hit->ray->direction), hit->normal);
	if (fuzz > 0.0f)
		reflected_dir = v3f_add(reflected_dir, v3f_scale(random_in_hemisphere(hit->normal), fuzz));
	reflected_ray.origin = v3f_add(hit->hit, v3f_scale(hit->normal, 0.001f));
	reflected_ray.direction = v3f_norm(reflected_dir);
	if (v3f_dot(reflected_ray.direction, hit->normal) > 0.0f)
	{
		result = trace(&reflected_ray, sc, depth - 1);
		return (v3f_mul(albedo, result));
	}
	return (g_v3f_zero);
}
