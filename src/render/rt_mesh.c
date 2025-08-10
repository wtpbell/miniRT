/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_mesh.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/02 17:28:29 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/10 21:03:31 by jboon         ########   odam.nl         */
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
	tri = obj->mesh.triangles + res->face_index;
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
		tri_uv_texcoord(obj->mesh.triangles + res->face_index,
			res->tri_weight
		));
}

static t_result	intersect_bhv(t_ray *ray, t_mesh *mesh, uint32_t idx, t_v2f t)
{
	t_bhv_node			*node;
	uint32_t			i;
	t_tri				*tri;
	t_result			curr;
	t_result			final;

	final.t = FLT_MAX;
	final.face_index = -1;
	node = &mesh->bhv[idx];
	if (!aabb_intersect(ray, &node->box))
		return (final);
	if (node->prim_count != 0)
	{
		i = 0;
		while (i < node->prim_count)
		{
			tri = mesh->triangles + node->first_prim + i;
			if (tri_intersect(tri, ray, t, &curr))
			{
				final = curr;
				final.face_index = node->first_prim + i;
				t.y = curr.t;
			}
			++i;
		}
		return (final);
	}
	curr = intersect_bhv(ray, mesh, node->left, t);
	if (curr.t < final.t)
	{
		final = curr;
		t.y = final.t;
	}
	curr = intersect_bhv(ray, mesh, node->right, t);
	if (curr.t < final.t)
		return (curr);
	return (final);
}

int	mesh_intersect(t_obj *obj, t_ray *ray, t_v2f t, t_result *res)
{
	// int			i;
	// t_result	curr;
	t_ray		local_ray;

	local_ray.origin = mul_v3_m4x4(ray->origin, obj->t.to_obj);
	local_ray.direction = mul_dir_m4x4(ray->direction, obj->t.to_obj);
	*res = intersect_bhv(&local_ray, &obj->mesh, 0, t);
	// if (!aabb_intersect(&local_ray, &obj->mesh.box))
	// 	return (false);
	// i = 0;
	// res->face_index = -1;
	// while (i < obj->mesh.tri_count)
	// {
	// 	if (tri_intersect(obj->mesh.triangles + i, &local_ray, t, &curr))
	// 	{
	// 		*res = curr;
	// 		t.y = curr.t;
	// 		res->face_index = i;
	// 	}
	// 	++i;
	// }
	return (res->face_index != -1);
}
