/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   aabb.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/07 11:13:10 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/12 17:26:41 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "rt_math.h"
#include "ray.h"
#include "vector.h"

static inline void	set_next_clip_plane(t_v3f *t_min, t_v3f *t_max)
{
	if (t_min->y > t_min->x)
		t_min->x = t_min->y;
	if (t_max->y < t_max->x)
		t_max->x = t_max->y;
}

bool	aabb_intersect(t_ray *ray, t_aabb *box)
{
	t_v3f		t_min;
	t_v3f		t_max;
	const t_v3f	invdir = init_v3f(
			1.0f / ray->direction.x,
			1.0f / ray->direction.y,
			1.0f / ray->direction.z);

	t_min.x = (box->min.x - ray->origin.x) * invdir.x;
	t_max.x = (box->max.x - ray->origin.x) * invdir.x;
	t_min.y = (box->min.y - ray->origin.y) * invdir.y;
	t_max.y = (box->max.y - ray->origin.y) * invdir.y;
	if (invdir.x < 0.0f)
		ft_swapf(&t_min.x, &t_max.x);
	if (invdir.y < 0.0f)
		ft_swapf(&t_min.y, &t_max.y);
	if ((t_min.x > t_max.y) || (t_min.y > t_max.x))
		return (false);
	set_next_clip_plane(&t_min, &t_max);
	t_min.z = (box->min.z - ray->origin.z) * invdir.z;
	t_max.z = (box->max.z - ray->origin.z) * invdir.z;
	if (invdir.z < 0.0f)
		ft_swapf(&t_min.z, &t_max.z);
	return (!((t_min.x > t_max.z) || (t_min.z > t_max.x)));
}
