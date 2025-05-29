/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_triangle.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/29 14:00:37 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/29 14:51:51 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ray.h"
#include "scene.h"
#include "rt_math.h"

#include "minirt.h"

// TODO: normal should be be based on the side (front/back) it hits of the
//		 triangle. Now it always return the 'outside' normal.
t_v3f	triangle_normal(t_obj *obj, t_v3f point)
{
	(void)point;
	return (obj->t.dir);
}

int	triangle_intersect(t_obj *obj, t_ray *ray, t_v2f t, float *dst)
{
	float	d;
	float	denom;
	float	t0;
	t_tri	*tri;
	t_v3f	p;

	denom = v3f_dot(obj->t.dir, ray->direction);
	if (fabs(denom) > FLT_SML)
	{
		d = -v3f_dot(obj->t.dir, obj->u_shape.tri.v0);
		t0 = -(v3f_dot(obj->t.dir, ray->origin) + d / denom);
		*dst = t0;
		if (t0 > t.x && t0 < t.y)
		{
			p = v3f_add(ray->origin, v3f_scale(ray->direction, t0));
			tri = &obj->u_shape.tri;
			// point is on the plane; and now check if it is within the triangle
			return (v3f_dot(obj->t.dir, v3f_cross(v3f_sub(tri->v1, tri->v0), v3f_sub(p, tri->v0))) > 0
				&& v3f_dot(obj->t.dir, v3f_cross(v3f_sub(tri->v2, tri->v1), v3f_sub(p, tri->v1))) > 0
				&& v3f_dot(obj->t.dir, v3f_cross(v3f_sub(tri->v0, tri->v2), v3f_sub(p, tri->v2))) > 0
			);
		}
	}
	return (0);
}
