/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_triangle.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/29 14:00:37 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/29 14:06:05 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ray.h"
#include "scene.h"
#include "rt_math.h"

// TODO: normal should be be based on the side (front/back) it hits of the
//		 triangle. Now it always return the 'outside' normal.
t_v3f	triangle_normal(t_obj *obj, t_v3f point)
{
	(void)point;
	return (obj->t.dir);
}

int	triangle_intersect(t_obj *obj, t_ray *ray, t_v2f t, float *dst)
{
	(void)obj;
	(void)ray;
	(void)t;
	(void)dst;
	return (0);
}
