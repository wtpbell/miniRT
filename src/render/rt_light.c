/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_light.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/31 19:11:17 by bewong        #+#    #+#                 */
/*   Updated: 2025/06/22 10:40:28 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "light.h"
#include "material.h"
#include "minirt.h"
#include "ray.h"
#include "rt_math.h"

t_v3f	apply_ambient(t_v3f base_col, t_light *light)
{
	return (v3f_scale(v3f_mul(base_col, light->color), light->intensity));
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

static float	get_specular(t_lighting *lt, t_ray_hit *hit)
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

	init_lighting(&lt, hit, light, scene->camera.t.pos);
	inten = lt.inten / (1.0f + 0.02f * lt.dist + 0.0002f * lt.dist * lt.dist);
	ray.origin = hit->hit;
	ray.direction = v3f_sub(light->pos, hit->hit);
	shadow_dist = lt.dist;
	if (find_intersection(&ray, scene, &shadow_dist) && shadow_dist < 1.0f)
		return (g_v3f_zero);
	lt.diffuse = calculate_diffuse(&lt);
	lt.specular = get_specular(&lt, hit);
	color = v3f_scale(v3f_mul(hit->hit_color, light->color), lt.diffuse);
	color = v3f_add(color, v3f_scale(v3f_mul(hit->hit_color, light->color), lt.specular));
	return (v3f_clampf01(v3f_scale(color, inten)));
}

t_v3f	apply_spot(t_scene *scene, t_ray_hit *hit, t_light *light)
{
	float	att;
	float	l_dot_wi;
	t_v3f	wi;

	wi = v3f_norm(v3f_sub(hit->hit, light->pos));
	l_dot_wi = v3f_dot(light->spot.dir, wi);
	if (l_dot_wi < light->spot.outer)
		return (g_v3f_zero);
	att = ft_clampf01((l_dot_wi - light->spot.outer) / (light->spot.inner - light->spot.outer));
	return (v3f_scale(apply_point(scene, hit, light), att * att));
}
