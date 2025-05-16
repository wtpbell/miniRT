/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_sp.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/16 16:04:41 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/16 18:07:24 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ray.h"
#include "scene.h"
#include "rt_math.h"

static bool	solve_quadratic(t_v3f *abc, float *x0, float *x1)
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

bool	sphere_intersect(t_obj *obj, t_ray *ray, float *dst)
{
	t_v3f		L;
	t_v3f		abc;
	float		t0;
	float		t1;
	float		r;

	r = obj->shape.sp.radius;
	L = v3f_sub(ray->origin, obj->t.pos);
	abc.x = v3f_dot(ray->direction, ray->direction);
	abc.y = 2.0f * v3f_dot(ray->direction, L);
	abc.z = v3f_dot(L, L) - r * r;
	if (!solve_quadratic(&abc, &t0, &t1))
		return (false);
	if (t0 < 0)
	{
		if (t1 < 0)
			return (false);
		t0 = t1;
	}
	*dst = t0;
	return (true);
}
