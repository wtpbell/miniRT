/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 12:17:53 by bewong            #+#    #+#             */
/*   Updated: 2025/06/13 13:06:23 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "light.h"

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

static t_v3f	handle_ambient_light(t_light *light, t_ray_hit *hit_info,
			t_scene *scene, t_v3f current_col)
{
	(void)scene;
	return (v3f_add(current_col, apply_ambient(hit_info->obj->r.color, light)));
}

static t_v3f	handle_point_light(t_light *light, t_ray_hit *hit_info,
			t_scene *scene, t_v3f current_col)
{
	return (v3f_add(current_col, apply_point(scene, hit_info, light)));
}

t_v3f	compute_lighting(t_ray_hit *hit_info, t_scene *scene)
{
	t_v3f								col;
	t_light								*light;
	int									i;
	static const t_light_handler_func	handlers[] = {
	[LIGHT_AMBIENT] = handle_ambient_light,
	[LIGHT_POINT] = handle_point_light,
	};

	i = 0;
	col = g_v3f_zero;
	while (i < scene->lights.size)
	{
		light = (t_light *)scene->lights.items[i];
		if (light->type >= 0 && light->type
			< (int)(sizeof(handlers) / sizeof(handlers[0]))
			&& handlers[light->type])
			col = handlers[light->type](light, hit_info, scene, col);
		i++;
	}
	return (col);
}
