/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_mesh.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/02 17:28:29 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/10 10:25:56 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "scene.h"
#include "rt_math.h"
#include "ray.h"
#include "minirt.h"

t_v3f	mesh_normal(t_obj *obj, t_v3f point, t_result *res)
{
	const float	w = 1.0f - res->tri_weight.u - res->tri_weight.v;
	t_tri		*tri;

	(void)point;
	tri = (t_tri *)obj->mesh.triangles.items[res->face_index];
	return (mul_dir_m4x4(
			v3f_add(v3f_scale(tri->vn2, res->tri_weight.v),
				v3f_add(v3f_scale(tri->vn1, res->tri_weight.u),
					v3f_scale(tri->vn0, w))),
			obj->t.to_world));
}

t_v2f	mesh_texcoord(t_obj *obj, t_v3f point, t_result *res)
{
	(void)point;
	return (
		tri_uv_texcoord((t_tri *)obj->mesh.triangles.items[res->face_index],
			res->tri_weight
		));
}

int	mesh_intersect(t_obj *obj, t_ray *ray, t_v2f t, t_result *res)
{
	int			i;
	t_tri		*tri;
	t_result	curr;
	t_ray		local_ray;

	local_ray.origin = mul_v3_m4x4(ray->origin, obj->t.to_obj);
	local_ray.direction = mul_dir_m4x4(ray->direction, obj->t.to_obj);
	if (!aabb_intersect(&local_ray, &obj->mesh.box))
		return (false);
	i = 0;
	res->face_index = -1;
	while (i < obj->mesh.triangles.size)
	{
		tri = (t_tri *)obj->mesh.triangles.items[i];
		if (tri_intersect(tri, &local_ray, t, &curr))
		{
			*res = curr;
			t.y = curr.t;
			res->face_index = i;
		}
		++i;
	}
	return (res->face_index != -1);
}
