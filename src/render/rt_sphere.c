/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_sp.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/16 16:04:41 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/28 12:50:36 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ray.h"
#include "rt_math.h"
#include "scene.h"

bool	solve_quadratic(t_v3f *abc, float *x0, float *x1)
{
	float	dis;
	float	q;

	dis = abc->y * abc->y - 4 * abc->x * abc->z;
	if (dis < 0)
		return (false);
	else if (dis == 0)
	{
		*x0 = -0.5f * abc->y / abc->x;
		*x1 = *x0;
	}
	else
	{
		q = -0.5f * (abc->y + ft_signf(abc->y) * sqrtf(dis));
		*x0 = q / abc->x;
		*x1 = abc->z / q;
	}
	if (*x0 > *x1)
		ft_swapf(x0, x1);
	return (true);
}

t_v3f	sphere_normal(t_obj *obj, t_v3f point)
{
	return (v3f_norm(v3f_sub(point, obj->t.pos)));
}


/*
	Spherical Coordinates (p, theta, phi)
	p (rho) = distance between point and the origin (radius)
	theta = angle counter clockwise from the polar/positive x-axis in the xy-plane
	phi = the angle between the positive z-axis and the point

	p^2 = x^2 + y^2 + z^2
	tan theta = y/x
	cos phi = z / (sqrt(p^2))

	By convention the z-axis is considered the up axis, but our ray tracer it will be the y-axis
*/
t_v2f	sphere_texcoord(t_obj *obj, t_v3f world_point)
{
	t_v3f	local_point;
	float	theta;
	float	phi;

	local_point = v3f_sub(world_point, obj->t.pos);
	theta = atan2f(local_point.z, local_point.x);
	phi = acosf(ft_clampf(local_point.y / obj->sp.radius, -1.0f, 1.0f));
	return (init_v2f((theta + M_PI) / TAU, phi / TAU));
}

int	sphere_intersect(t_obj *obj, t_ray *ray, t_v2f t, float *dst)
{
	t_v3f		oc;
	t_v3f		abc;
	float		t0;
	float		t1;
	float		r;

	r = obj->sp.radius;
	oc = v3f_sub(ray->origin, obj->t.pos);
	abc.x = v3f_dot(ray->direction, ray->direction);
	abc.y = 2.0f * v3f_dot(ray->direction, oc);
	abc.z = v3f_dot(oc, oc) - r * r;
	if (!solve_quadratic(&abc, &t0, &t1))
		return (0);
	if (t0 < t.x)
	{
		if (t1 < t.x)
			return (0);
		t0 = t1;
	}
	*dst = t0;
	return (t0 < t.y);
}
