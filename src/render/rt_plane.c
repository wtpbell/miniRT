/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_plane.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/19 10:33:19 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/19 10:57:08 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ray.h"
#include "scene.h"
#include "rt_math.h"

int	plane_intersect(t_obj *obj, t_ray *ray, float *dst)
{
	float	denom;

	denom = v3f_dot(ray->direction, obj->t.dir);
	if (denom > FLT_EPI)
	{
		*dst = v3f_dot(v3f_sub(obj->t.pos, ray->origin), obj->t.dir) / denom;
		return (*dst >= 0);
	}
	return (false);
}
