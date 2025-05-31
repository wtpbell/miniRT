/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_plane.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 10:33:19 by jboon             #+#    #+#             */
/*   Updated: 2025/05/31 23:13:55 by bewong           ###   ########.fr       */
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
