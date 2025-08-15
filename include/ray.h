/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:37:29 by bewong            #+#    #+#             */
/*   Updated: 2025/08/04 14:51:46 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAY_H
# define RAY_H

# include "vector.h"
# include "rt_types.h"

struct s_ray
{
	t_v3f	origin;
	t_v3f	direction;
};

struct s_ray_hit
{
	t_v3f	hit;
	t_v3f	normal;
	t_v2f	texcoord;
	t_v3f	hit_color;
	float	distance;
	bool	front_face;
	t_obj	*obj;
	t_ray	*ray;
	t_v2f	weight;
};

#endif
