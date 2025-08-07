/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_mesh.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/02 17:28:29 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/07 11:20:02 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "scene.h"
#include "rt_math.h"
#include "ray.h"
#include "minirt.h"

t_v3f	mesh_normal(t_obj *obj, t_v3f point, t_result *res)
{
	t_tri	*tri;
	float	u;
	float	v;
	float	w;

	(void)point;
	tri = (t_tri *)vector_get(&obj->mesh.triangles, res->face_index);
	if (tri == NULL)
		return (g_v3f_up);

	u = res->tri_weight.u;
	v = res->tri_weight.v;
	w = 1.0f - u - v;

	return (mul_dir_m4x4(
		v3f_add(v3f_scale(tri->vn2, v),
			v3f_add(v3f_scale(tri->vn1, u), v3f_scale(tri->vn0, w))),
			obj->t.to_world));
}

// TODO: Same code as triangle_texcoord
t_v2f	mesh_texcoord(t_obj *obj, t_v3f point, t_result *res)
{
	float	w;
	float	u;
	float	v;
	t_tri	*tri;

	(void)point;
	tri = (t_tri *)vector_get(&obj->mesh.triangles, res->face_index);
	if (tri == NULL)
		return (g_v2f_zero);

	tri = &obj->tri;
	u = res->tri_weight.u;
	v = res->tri_weight.v;
	w = 1.0f - u - v;

	return (init_v2f(
			w * tri->vt0.x + u * tri->vt1.x + v * tri->vt2.x,
			w * tri->vt0.y + u * tri->vt1.y + v * tri->vt2.y)
	);
}

int	mesh_intersect(t_obj *obj, t_ray *ray, t_v2f t, t_result *res)
{
	int		i;
	t_tri	*tri;
	t_ray	local_ray;

	local_ray.origin = mul_v3_m4x4(ray->origin, obj->t.to_obj);
	local_ray.direction = mul_dir_m4x4(ray->direction, obj->t.to_obj);
	if (!aabb_intersect(&local_ray, &obj->mesh.box))
		return (false);
	i = 0;
	while (i < obj->mesh.triangles.size)
	{
		tri = (t_tri *)obj->mesh.triangles.items[i];
		if (tri_intersect(tri, &local_ray, t, res))
		{
			res->face_index = i;
			return (true);
		}
		++i;
	}
	return (false);
}
