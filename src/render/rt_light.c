/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_light.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/31 19:11:17 by bewong        #+#    #+#                 */
/*   Updated: 2025/06/03 19:02:35 by bewong        ########   odam.nl         */
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
	lighting->obj_color = hit->obj->r.mat->albedo;
	lighting->inten = light->intensity;
	lighting->dist = v3f_mag(v3f_sub(light->pos, hit->hit));
}

float	calculate_diffuse(t_lighting *lighting)
{
	return (ft_maxf(0.0f, v3f_dot(lighting->normal, lighting->light_dir)));
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

/**quadratic falloff: intensity / (1 + a*d + b*d²)
combines diffuse and specular, multiplies by albedo and light
color and intensity, finally clamps to 0-1 **/
t_v3f	apply_point(t_scene *scene, t_ray_hit *hit, t_light *light)
{
	t_ray		ray;
	t_lighting	lt;
	float		inten;
	float		shadow_dist;
	t_v3f		color;

	init_lighting(&lt, hit, light, scene->camera.t.pos);
	inten = lt.inten / (1.0f + 0.1f
			* lt.dist + 0.01f * lt.dist * lt.dist);
	ray.origin = hit->hit;
	ray.direction = v3f_sub(light->pos, hit->hit);
	shadow_dist = lt.dist;
	if (find_intersection(&ray, scene, &shadow_dist) && shadow_dist < 1.0f)
		return ((t_v3f){{0.0f, 0.0f, 0.0f}}); // if sth between hit and light, in shadow
	lt.diffuse = calculate_diffuse(&lt);
	if (hit->obj->r.mat->lamb.specular > 0.0f) // only calculate specular if there is any
		lt.specular = calculate_specular(&lt, hit->obj->r.mat->lamb.shininess,
				hit->obj->r.mat->lamb.specular);
	else
		lt.specular = 0.0f;
	color = v3f_scale(v3f_mul(hit->obj->r.mat->albedo, light->color),
			lt.diffuse);
	return (v3f_clampf01(v3f_scale(v3f_add(
					color, v3f_scale(light->color, lt.specular)), inten)));
}
