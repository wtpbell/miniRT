/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_triangle.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/29 14:00:37 by jboon         #+#    #+#                 */
/*   Updated: 2025/06/17 21:33:13 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ray.h"
#include "rt_math.h"
#include "scene.h"

static void	set_minmax(t_tri *tri, t_v2f *min, t_v2f *max)
{
	min->x = fminf(fminf(tri->vt0.x, tri->vt1.x), tri->vt2.x);
	min->y = fminf(fminf(tri->vt0.y, tri->vt1.y), tri->vt2.y);
	max->x = fmaxf(fmaxf(tri->vt0.x, tri->vt1.x), tri->vt2.x);
	max->y = fmaxf(fmaxf(tri->vt0.y, tri->vt1.y), tri->vt2.y);
}

t_v3f	triangle_normal(t_obj *obj, t_v3f point)
{
	(void)point;
	return (obj->t.dir);
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

t_v2f	triangle_texcoord(t_obj *obj, t_v3f world_point)
{
	float	w;
	float	u;
	float	v;
	t_tri	*tri;

	(void)world_point;
	tri = &obj->tri;
	u = tri->weight.u;
	v = tri->weight.v;
	w = 1.0f - u - v;
	return (init_v2f(
		w * tri->vt0.x + u * tri->vt1.x + v * tri->vt2.x,
		w * tri->vt0.y + u * tri->vt1.y + v * tri->vt2.y
	));
}

static int	is_within_triangle(t_tri *tri, t_ray *ray, t_tri_var *vars,
	float *dst)
{
	t_v3f	tvec;
	t_v3f	qvec;
	float	u;
	float	v;

	vars->det = 1.0f / vars->det;
	tvec = v3f_sub(ray->origin, tri->v0);
	u = v3f_dot(tvec, vars->pvec) * vars->det;
	if (u < 0.0f || u > 1.0f)
		return (0);
	qvec = v3f_cross(tvec, vars->v0v1);
	v = v3f_dot(ray->direction, qvec) * vars->det;
	if (v < 0.0f || (u + v) > 1.0f)
		return (0);
	tri->weight = init_v2f(u, v);
	*dst = v3f_dot(vars->v0v2, qvec) * vars->det;
	return (1);
}

/*
	This is the Möller-Trumbore algorithm for finding a ray-triangle
	intersection. It uses Cramer's Rule to find the t, u and v values.

	P = O + tD
	O + tD = (1 - u - v)A + uB + vC
	O + tD = A - uA - vA + uB + vC
	O + tD = A + u(B - A) + v(C - A)
	O - A = -tD + u(B - A) + v(C - A)
*/
int	triangle_intersect(t_obj *obj, t_ray *ray, t_v2f t, float *dst)
{
	t_tri		*tri;
	t_tri_var	var;

	tri = &obj->tri;
	var.v0v1 = v3f_sub(tri->v1, tri->v0);
	var.v0v2 = v3f_sub(tri->v2, tri->v0);
	var.pvec = v3f_cross(ray->direction, var.v0v2);
	var.det = v3f_dot(var.v0v1, var.pvec);
	if (fabs(var.det) > FLT_SML && is_within_triangle(tri, ray, &var, dst))
		return (*dst > t.x && *dst < t.y);
	return (0);
}
