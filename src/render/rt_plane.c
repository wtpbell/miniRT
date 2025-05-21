/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_plane.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 10:33:19 by jboon             #+#    #+#             */
/*   Updated: 2025/05/21 14:52:18 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ray.h"
#include "scene.h"
#include "rt_math.h"

t_v3f	plane_normal(t_obj *obj, t_v3f point)
{
	(void)obj;
	(void)point;
	return (obj->t.dir);
}

int plane_intersect(t_obj *obj, t_ray *ray, float *dst)
{
	float denom;
	float t;

	denom = v3f_dot(obj->t.dir, ray->direction);
	if (fabs(denom) > FLT_EPSILON)
	{
		t = v3f_dot(v3f_sub(obj->t.pos, ray->origin), obj->t.dir) / denom;
		if (t >= 0)  // Check if intersection is in front of the ray
		{
			*dst = t;
			return (true);
		}
	}
	return (false);
}
