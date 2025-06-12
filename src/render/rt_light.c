/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_light.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/31 19:11:17 by bewong        #+#    #+#                 */
/*   Updated: 2025/06/12 10:06:02 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "light.h"
#include "rt_math.h"
#include "material.h"
#include "minirt.h"

t_v3f	apply_ambient(t_v3f base_col, t_light *light)
{
	return (v3f_scale(v3f_mul(base_col, light->color), light->intensity));
}

void	init_lighting(t_lighting *lighting, t_ray_hit *hit,
			t_light *light, t_v3f camera_pos)
{
	lighting->light_dir = v3f_norm(v3f_sub(light->pos, hit->hit));
	lighting->view_dir = v3f_norm(v3f_sub(camera_pos, hit->hit));
	lighting->hit_point = hit->hit;
	lighting->normal = hit->normal;
	lighting->light_color = light->color;
	lighting->obj_color = hit->obj->r.color;
	lighting->inten = light->intensity;
	lighting->dist = v3f_mag(v3f_sub(light->pos, hit->hit));
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

float	calculate_specular(t_lighting *lighting,
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

/**quadratic falloff: inten / (1 + a*d + b*d²)
combines diffuse and specular, multiplies by albedo and light
color and inten, finally clamps to 0-1 **/
t_v3f	apply_point(t_scene *scene, t_ray_hit *hit, t_light *light)
{
	t_ray		ray;
	t_lighting	lt;
	float		inten;
	float		shadow_dist;
	t_v3f		color;

	ft_bzero(&lt, sizeof(t_lighting));
	init_lighting(&lt, hit, light, scene->camera.t.pos);
	inten = lt.inten / (1.0f + 0.02f * lt.dist + 0.0002f * lt.dist * lt.dist);
	ray.origin = hit->hit;
	ray.direction = v3f_sub(light->pos, hit->hit);
	shadow_dist = lt.dist;
	if (find_intersection(&ray, scene, &shadow_dist) && shadow_dist < 1.0f)
		return (g_v3f_zero);
	lt.diffuse = calculate_diffuse(&lt);
	if (hit->obj->r.mat->type == MAT_LAMBERTIAN && hit->obj->r.mat->lamb.specular > 0.0f)
		lt.specular = calculate_specular(&lt, hit->obj->r.mat->lamb.shininess, hit->obj->r.mat->lamb.specular);
	else if (hit->obj->r.mat->type == MAT_DIELECTRIC)
	{
		float cos_theta = v3f_dot(lt.normal, lt.view_dir);
		lt.specular = schlick(cos_theta, hit->obj->r.mat->diel.ir);
	}
	else if (hit->obj->r.mat->type == MAT_METAL)
	{
		lt.specular = calculate_specular(&lt, 100.0f, 1.0f);
		lt.specular *= (1.0f - hit->obj->r.mat->metal.fuzz);
	}
	else
		lt.specular = 0.0f;
	color = v3f_scale(v3f_mul(lt.obj_color, light->color), lt.diffuse);
	return v3f_clampf01(v3f_scale(v3f_add(color, v3f_scale(light->color, lt.specular)), inten));
}

