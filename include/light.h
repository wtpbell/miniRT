/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 19:14:47 by bewong            #+#    #+#             */
/*   Updated: 2025/05/31 19:14:47 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIGHT_H
# define LIGHT_H

# include "scene.h"

typedef struct s_lighting
{
	t_v3f	light_dir;
	t_v3f	view_dir;
	t_v3f	hit_point;
	t_v3f	normal;
	t_v3f	light_color;
	t_v3f	obj_color;
	float	intensity;
	float	distance;
	float	diffuse;
	float	specular;
}	t_lighting;

void	init_lighting(t_lighting *lighting, t_ray_hit *hit,
			t_light *light, t_v3f view_pos);
float	calculate_diffuse(t_lighting *lighting);
float	calculate_specular(t_lighting *lighting, float shininess,
			float specular_strength);
t_v3f	apply_ambient(t_v3f base_col, t_light *light);
t_v3f	apply_point(t_scene *scene, t_ray_hit *hit, t_light *light);

#endif
