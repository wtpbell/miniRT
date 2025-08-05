/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_mesh.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/02 17:28:29 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/05 10:19:52 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "scene.h"
#include "rt_math.h"
#include "ray.h"
#include "minirt.h"


bool	aabb_intersect(t_ray *ray, t_aabb *box)
{
	// TODO: Make function static
	const t_v3f	invdir = init_v3f(
		1.0f / ray->direction.x,
		1.0f / ray->direction.y,
		1.0f / ray->direction.z
	);

	t_v3f	t_min;
	t_v3f	t_max;

	t_min.x = (box->min.x - ray->origin.x) * invdir.x;
	t_max.x = (box->max.x - ray->origin.x) * invdir.x;

	t_min.y = (box->min.y - ray->origin.y) * invdir.y;
	t_max.y = (box->max.y - ray->origin.y) * invdir.y;

	t_min.z = (box->min.z - ray->origin.z) * invdir.z;
	t_max.z = (box->max.z - ray->origin.z) * invdir.z;

	if (invdir.x < 0.0f)
		ft_swapf(&t_min.x, &t_max.x);
	if (invdir.y < 0.0f)
		ft_swapf(&t_min.y, &t_max.y);

	if ((t_min.x > t_max.y) || (t_min.y > t_max.x))
		return (false);

	if (t_min.y > t_min.x)
		t_min.x = t_min.y;
	if (t_max.y < t_max.x)
		t_max.x = t_max.y;

	if (invdir.z < 0.0f)
		ft_swapf(&t_min.z, &t_max.z);

	return (!((t_min.x > t_max.z) || (t_min.z > t_max.x)));
}

// TODO: Implementation needed
t_v3f	mesh_normal(t_obj *obj, t_v3f point, t_result *res)
{
	t_tri	*tri;

	(void)point;
	tri = (t_tri *)vector_get(&obj->mesh.triangles, res->face_index);
	if (tri == NULL)
		return (g_v3f_up);
	return (tri->vn0);
}

// TODO: Implementation needed
t_v2f	mesh_texcoord(t_obj *obj, t_v3f world_point, t_result *res)
{
	(void)obj;
	(void)world_point;
	(void)res;
	return (g_v2f_zero);
}

// TODO: Implementation needed
int	mesh_intersect(t_obj *obj, t_ray *ray, t_v2f t, t_result *res)
{
	int		i;
	t_tri	*tri;

	if (!aabb_intersect(ray, &obj->mesh.box))
		return (false);
	i = 0;
	while (i < obj->mesh.triangles.size)
	{
		tri = (t_tri *)obj->mesh.triangles.items[i];
		if (tri_intersect(tri, ray, t, res))
		{
			res->face_index = i;
			return (true);
		}
		++i;
	}
	return (false);
}
