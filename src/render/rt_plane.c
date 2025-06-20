/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_plane.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/19 10:33:19 by jboon         #+#    #+#                 */
/*   Updated: 2025/06/17 21:41:47 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ray.h"
#include "rt_math.h"
#include "scene.h"

t_v3f	plane_normal(t_obj *obj, t_v3f point)
{
	(void)point;
	return (obj->t.dir);
}

t_v2f	plane_texcoord(t_obj *obj, t_v3f world_point)
{
	t_v3f	local_point;

	local_point = mul_v3_m4x4(world_point, obj->t.to_obj);
	return (init_v2f(local_point.x, local_point.z));
}

int	plane_intersect(t_obj *obj, t_ray *ray, t_v2f t, float *dst)
{
	float	denom;
	float	t0;

	denom = v3f_dot(obj->t.dir, ray->direction);
	if (fabs(denom) > FLT_SML)
	{
		t0 = v3f_dot(v3f_sub(obj->t.pos, ray->origin), obj->t.dir) / denom;
		*dst = t0;
		if (t0 > t.x && t0 < t.y)
			return (true);
	}
	return (false);
}
