/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_plane.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 10:33:19 by jboon             #+#    #+#             */
/*   Updated: 2025/05/31 15:40:13 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene.h"
#include "rt_math.h"

t_v3f	plane_normal(t_obj *obj, t_v3f point)
{
	(void)point;
	return (v3f_norm(obj->t.dir));
}

int	plane_intersect(t_obj *obj, t_ray *ray, t_v2f t, float *dst)
{
	float	dot_nv;
	float	t0;
	t_v3f	plane_to_ray;

	dot_nv = v3f_dot(obj->t.dir, ray->direction);

	// Check if ray is parallel to plane
	if (fabs(dot_nv) < FLT_SML)
		return (false);

	// Calculate distance to intersection
	plane_to_ray = v3f_sub(ray->origin, obj->t.pos);
	t0 = -v3f_dot(plane_to_ray, obj->t.dir) / dot_nv;

	// Check if intersection is within bounds
	if (t0 <= t.x || t0 >= t.y)
		return (false);

	// Make sure we're not behind the ray origin
	if (t0 < 0.0f)
		return (false);

	*dst = t0;
	return (true);
}
