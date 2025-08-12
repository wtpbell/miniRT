/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_triangle_uv.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/02 18:33:02 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/12 17:28:26 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "rt_math.h"
#include "scene.h"
#include "ray.h"

static void	set_minmax(t_tri *tri, t_v2f *min, t_v2f *max)
{
	min->x = fminf(fminf(tri->vt0.x, tri->vt1.x), tri->vt2.x);
	min->y = fminf(fminf(tri->vt0.y, tri->vt1.y), tri->vt2.y);
	max->x = fmaxf(fmaxf(tri->vt0.x, tri->vt1.x), tri->vt2.x);
	max->y = fmaxf(fmaxf(tri->vt0.y, tri->vt1.y), tri->vt2.y);
}

void	generate_uv_vertices(t_tri *tri, t_mat4x4 local)
{
	float	aspect;
	t_v2f	min;
	t_v2f	max;
	t_v2f	size;

	tri->vt0 = mul_v3_m4x4(tri->v0, local);
	tri->vt1 = mul_v3_m4x4(tri->v1, local);
	tri->vt2 = mul_v3_m4x4(tri->v2, local);
	ft_swapf(&tri->vt0.y, &tri->vt0.z);
	ft_swapf(&tri->vt1.y, &tri->vt1.z);
	ft_swapf(&tri->vt2.y, &tri->vt2.z);
	set_minmax(tri, &min, &max);
	size.x = 1.0f / (max.x - min.x);
	size.y = 1.0f / (max.y - min.y);
	aspect = size.x / size.y;
	tri->vt0 = v3f_mul_v2f(v3f_sub_v2f(tri->vt0, min), size);
	tri->vt1 = v3f_mul_v2f(v3f_sub_v2f(tri->vt1, min), size);
	tri->vt2 = v3f_mul_v2f(v3f_sub_v2f(tri->vt2, min), size);
	tri->vt0.y = (1.0f - tri->vt0.y) * aspect;
	tri->vt1.y = (1.0f - tri->vt1.y) * aspect;
	tri->vt2.y = (1.0f - tri->vt2.y) * aspect;
}

t_v2f	tri_uv_texcoord(t_tri *tri, const t_v2f weight)
{
	const float	w = 1.0f - weight.u - weight.v;

	return (init_v2f(
			w * tri->vt0.x + weight.u * tri->vt1.x + weight.v * tri->vt2.x,
			w * tri->vt0.y + weight.u * tri->vt1.y + weight.v * tri->vt2.y)
		);
}

t_v2f	triangle_texcoord(t_obj *obj, t_v3f point, t_result *res)
{
	(void)point;
	return (tri_uv_texcoord(&obj->tri, res->tri_weight));
}
