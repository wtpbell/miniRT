/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_mesh.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/02 17:28:29 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/13 15:22:39 by jboon         ########   odam.nl         */
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

static t_result	intersect_leaf(t_bhv_node *node, t_mesh *mesh, t_ray *ray,
	t_v2f t)
{
	uint32_t	i;
	uint32_t	count;
	t_result	curr;
	t_result	final;

	final.t = FLT_MAX;
	final.face_index = -1;
	i = node->left;
	count = node->left + node->prim_count;
	while (i < count)
	{
		if (tri_intersect(mesh->triangles + i, ray, t, &curr))
		{
			t.y = curr.t;
			final = curr;
			final.face_index = i;
		}
		++i;
	}
	return (final);
}

static t_result	intersect_bvh(t_ray *ray, t_mesh *mesh, uint32_t idx, t_v2f t)
{
	t_result	left;
	t_result	right;

	if (!aabb_intersect(ray, &mesh->bvh[idx].box))
		return ((t_result){FLT_MAX, -1, g_v2f_zero});
	if (mesh->bvh[idx].prim_count != 0)
		return (intersect_leaf(mesh->bvh + idx, mesh, ray, t));
	left = intersect_bvh(ray, mesh, mesh->bvh[idx].left, t);
	if (left.t < t.y)
		t.y = left.t;
	right = intersect_bvh(ray, mesh, mesh->bvh[idx].left + 1, t);
	if (left.t < right.t)
		return (left);
	return (right);
}

int	mesh_intersect(t_obj *obj, t_ray *ray, t_v2f t, t_result *res)
{
	t_ray	local_ray;

	local_ray.origin = mul_v3_m4x4(ray->origin, obj->t.to_obj);
	local_ray.direction = mul_dir_m4x4(ray->direction, obj->t.to_obj);
	*res = intersect_bvh(&local_ray, &obj->mesh, 0, t);
	return (res->face_index != -1);
}
