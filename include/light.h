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

# include "rt_types.h"
# include "minirt.h"

struct s_lighting
{
	t_v3f	light_dir;
	t_v3f	view_dir;
	t_v3f	hit_point;
	t_v3f	normal;
	t_v3f	light_color;
	t_v3f	obj_color;
	float	inten;
	float	dist;
	float	diffuse;
	float	specular;
};

float	calculate_diffuse(t_lighting *lighting);
float	get_specular(t_lighting *lt, t_ray_hit *hit);
t_v3f	compute_lighting(t_ray_hit *hit_info, t_scene *scene);

#endif
