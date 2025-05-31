/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_light.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 19:11:17 by bewong            #+#    #+#             */
/*   Updated: 2025/05/31 23:44:28 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "light.h"
#include "rt_math.h"

t_col32	apply_ambient(t_col32 base_col, t_light *light)
{
	return (v3f_to_col32((t_v3f){
			.x = col32_to_v3f(base_col).x
			* col32_to_v3f(light->col).x * light->intensity,
			.y = col32_to_v3f(base_col).y
			* col32_to_v3f(light->col).y * light->intensity,
			.z = col32_to_v3f(base_col).z
			* col32_to_v3f(light->col).z * light->intensity
		}));
}

void	init_lighting(t_lighting *lighting, t_ray_hit *hit,
			t_light *light, t_v3f camera_pos)
{
	lighting->light_dir = v3f_norm(v3f_sub(light->pos, hit->hit));
	lighting->view_dir = v3f_norm(v3f_sub(camera_pos, hit->hit));
	lighting->hit_point = hit->hit;
	lighting->normal = hit->normal;
	lighting->light_color = light->col;
	lighting->obj_color = hit->obj->r.col;
	lighting->intensity = light->intensity;
	lighting->distance = v3f_mag(v3f_sub(light->pos, hit->hit));
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

t_col32	apply_point(t_scene *scene, t_ray_hit *hit, t_light *light)
{
	t_ray		ray;
	t_lighting	lt;
	t_v3f		colors[2];
	float		intensity;
	float		shadow_dist;

	init_lighting(&lt, hit, light, scene->camera.t.pos);
	intensity = lt.intensity / (1.0f + 0.1f
			* lt.distance + 0.01f * lt.distance * lt.distance);
	ray.origin = v3f_add(hit->hit, v3f_scale(hit->normal, BIAS));
	ray.direction = v3f_norm(v3f_sub(light->pos, hit->hit));
	shadow_dist = lt.distance;
	if (find_intersection(&ray, scene, &shadow_dist)
		&& shadow_dist < lt.distance - BIAS)
		return (init_col32(0, 0, 0, 255));
	lt.diffuse = calculate_diffuse(&lt);
	lt.specular = calculate_specular(&lt,
			hit->obj->r.mat->lamb.shininess,
			hit->obj->r.mat->lamb.specular);
	colors[0] = col32_to_v3f(lt.obj_color);
	colors[1] = col32_to_v3f(lt.light_color);
	return (v3f_to_col32((t_v3f){
			.x = (colors[0].x * colors[1].x * lt.diffuse
				+ colors[1].x * lt.specular) * intensity,
			.y = (colors[0].y * colors[1].y * lt.diffuse
				+ colors[1].y * lt.specular) * intensity,
			.z = (colors[0].z * colors[1].z * lt.diffuse
				+ colors[1].z * lt.specular) * intensity
		}));
}
