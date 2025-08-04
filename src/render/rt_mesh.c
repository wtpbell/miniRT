/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_mesh.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/02 17:28:29 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/04 18:30:07 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "scene.h"
#include "rt_math.h"
#include "ray.h"
#include "minirt.h"


bool	aabb_intersect(t_ray *ray, t_aabb *box)
{
	// TODO: Make function static
	const t_v3f	invdir = init_v3f(1.0f / ray->direction.x,
		1.0f / ray->direction.y,
		1.0f / ray->direction.z
	);
	t_v2f	is_x;
	t_v2f	is_y;
	t_v2f	is_z;

	// min
	is_x.x = (box->min.x - ray->origin.x) * invdir.x;
	is_y.x = (box->min.y - ray->origin.y) * invdir.y;
	is_z.x = (box->min.z - ray->origin.z) * invdir.z;

	// max
	is_x.y = (box->max.x - ray->origin.x) * invdir.x;
	is_y.y = (box->max.y - ray->origin.y) * invdir.y;
	is_z.y = (box->max.z - ray->origin.z) * invdir.z;

	if (invdir.x >= 0.0f)
		ft_swapf(&is_x.x, &is_x.y);
	if (invdir.y >= 0.0f)
		ft_swapf(&is_y.x, &is_y.y);
	if (invdir.z >= 0.0f)
		ft_swapf(&is_z.x, &is_z.y);

	if ((is_x.x > is_y.y) || (is_y.x > is_x.x))
		return (false);

	if (is_y.x > is_x.x)
		is_x.x = is_y.x;
	if (is_y.y < is_x.y)
		is_x.y = is_y.y;
	return (!((is_x.x > is_z.y) || (is_z.x > is_x.x)));
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

	// if (!aabb_intersect(ray, &obj->mesh.box))
	// 	return (false);
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
