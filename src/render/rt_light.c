/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_light.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 19:11:17 by bewong            #+#    #+#             */
/*   Updated: 2025/06/26 17:01:38 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "light.h"
#include "ray.h"
#include "rt_math.h"

#define LU_MAX_SIZE	3

static void	init_lighting(t_lighting *lighting, t_ray_hit *hit,
	t_light *light, t_v3f camera_pos)
{
	lighting->light_dir = v3f_norm(v3f_sub(light->pos, hit->hit));
	lighting->view_dir = v3f_norm(v3f_sub(camera_pos, hit->hit));
	lighting->hit_point = hit->hit;
	lighting->normal = hit->normal;
	lighting->light_color = light->color;
	lighting->obj_color = hit->hit_color;
	lighting->inten = light->intensity;
	lighting->dist = v3f_mag(v3f_sub(light->pos, hit->hit));
	lighting->diffuse = 0.0f;
	lighting->specular = 0.0f;
}

static t_v3f	apply_ambient(t_scene *scene, t_ray_hit *hit_info,
	t_light *light)
{
	(void)scene;
	return (v3f_scale(
			v3f_mul(hit_info->hit_color, light->color), light->intensity));
}

/**quadratic falloff: inten / (1 + a*d + b*d²)
combines diffuse and specular, multiplies by albedo and light
color and inten, finally clamps to 0-1 **/
static t_v3f	apply_point(t_scene *scene, t_ray_hit *hit, t_light *light)
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
	color = v3f_add(color,
			v3f_scale(v3f_mul(hit->hit_color, light->color), lt.specular));
	return (v3f_clampf01(v3f_scale(color, inten)));
}

static t_v3f	apply_spot(t_scene *scene, t_ray_hit *hit, t_light *light)
{
	float	att;
	float	l_dot_wi;
	t_v3f	wi;

	wi = v3f_norm(v3f_sub(hit->hit, light->pos));
	l_dot_wi = v3f_dot(light->spot.dir, wi);
	if (l_dot_wi < light->spot.outer)
		return (g_v3f_zero);
	att = ft_clampf01(
			(l_dot_wi - light->spot.outer)
			/ (light->spot.inner - light->spot.outer));
	return (v3f_scale(apply_point(scene, hit, light), att * att));
}

t_v3f	compute_lighting(t_ray_hit *hit_info, t_scene *scene)
{
	t_v3f				col;
	t_light				*light;
	int					i;
	const t_apply_light	handlers[LU_MAX_SIZE] = {
	[LIGHT_AMBIENT] = apply_ambient,
	[LIGHT_POINT] = apply_point,
	[LIGHT_SPOT] = apply_spot,
	};

	i = 0;
	col = g_v3f_zero;
	while (i < scene->lights.size)
	{
		light = (t_light *)scene->lights.items[i];
		if (light->type >= 0 && light->type < LU_MAX_SIZE
			&& handlers[light->type])
			col = v3f_add(col, handlers[light->type](scene, hit_info, light));
		i++;
	}
	return (col);
}
