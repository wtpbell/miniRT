/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_mesh.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/02 17:28:29 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/04 23:35:55 by jboon         ########   odam.nl         */
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
	t_v2f	t;
	t_v2f	ty;
	t_v2f	tz;

	// min
	t.x = (box->min.x - ray->origin.x) * invdir.x;
	ty.x = (box->min.y - ray->origin.y) * invdir.y;
	tz.x = (box->min.z - ray->origin.z) * invdir.z;

	// max
	t.y = (box->max.x - ray->origin.x) * invdir.x;
	ty.y = (box->max.y - ray->origin.y) * invdir.y;
	tz.y = (box->max.z - ray->origin.z) * invdir.z;

	if (invdir.x < 0.0f)
		ft_swapf(&t.x, &t.y);
	if (invdir.y < 0.0f)
		ft_swapf(&ty.x, &ty.y);

	if ((t.x > ty.y) || (ty.x > t.x))
		return (false);

	if (ty.x > t.x)
		t.x = ty.x;
	if (ty.y < t.y)
		t.y = ty.y;

	if (invdir.z < 0.0f)
		ft_swapf(&tz.x, &tz.y);
	
	return (!((t.x > tz.y) || (tz.x > t.x)));
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
