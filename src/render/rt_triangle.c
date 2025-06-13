/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_triangle.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/29 14:00:37 by jboon         #+#    #+#                 */
/*   Updated: 2025/06/13 19:09:09 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ray.h"
#include "rt_math.h"
#include "scene.h"

t_v3f	triangle_normal(t_obj *obj, t_v3f point)
{
	(void)point;
	return (obj->t.dir);
}

void	set_minmax(t_v2f vt[3], t_v2f *min, t_v2f *max)
{
	min->x = fminf(fminf(vt[0].x, vt[1].x), vt[2].x);
	min->y = fminf(fminf(vt[0].y, vt[1].y), vt[2].y);
	max->x = fmaxf(fmaxf(vt[0].x, vt[1].x), vt[2].x);
	max->y = fmaxf(fmaxf(vt[0].y, vt[1].y), vt[2].y);
}

t_v2f	project(const t_v3f *point, const t_v3f *orig, const t_v3f *u, const t_v3f *v)
{
	t_v3f	rel;

	rel = v3f_sub(*point, *orig);
	return (init_v2f(v3f_dot(rel, *u), v3f_dot(rel, *v)));
}

int	g_is_printed = 0;
#include <stdio.h>

/* Keyword: planar projection */
// TODO: Generate this at init of the triangle
void	generate_uv_vertices(t_v2f vt[3], t_tri *tri, t_v3f forw)
{
	int		i;
	t_v3f	right;
	t_v3f	up;
	t_v2f	min;
	t_v2f	max;

	// TODO: Code duplication! Check plane_texcoord, obj_to_world, view_matrix
	if (fabsf(v3f_dot(g_v3f_up, forw)) > .99f)
		right = v3f_norm(v3f_cross(forw, g_v3f_foward));
	else
		right = v3f_norm(v3f_cross(forw, g_v3f_up));
	up = v3f_cross(forw, right);

	vt[0] = project(&tri->v0, &tri->v0, &right, &up);
	vt[1] = project(&tri->v1, &tri->v0, &right, &up);
	vt[2] = project(&tri->v2, &tri->v0, &right, &up);

	// vt[0] = init_v2f(v3f_dot(tri->v0, right), v3f_dot(tri->v0, up));
	// vt[1] = init_v2f(v3f_dot(tri->v1, right), v3f_dot(tri->v1, up));
	// vt[2] = init_v2f(v3f_dot(tri->v2, right), v3f_dot(tri->v2, up));

	float	aspect;

	aspect = (max.x - min.x) / (max.y - min.y);

	i = 0;
	set_minmax(vt, &min, &max);
	// possible division by zero
	while (i < 3)
	{
		vt[i].x = 1.0f - (vt[i].x - min.x) / (max.x - min.x);
		vt[i].y = 1.0f - (vt[i].y - min.y) / (max.y - min.y);
		++i;
	}

	if (g_is_printed == 0)
	{
		printf("vt0<%f, %f>, vt1<%f, %f>, vt2<%f, %f>\n",
			vt[0].x, vt[0].y,
			vt[1].x, vt[1].y,
			vt[2].x, vt[2].y);
		g_is_printed = 1;
	}
}

// TODO: Instead of re-calculating the barycentric-coordinates, store them somewhere when checking for intersection
t_v2f	triangle_texcoord(t_obj *obj, t_v3f world_point)
{
	t_tri	*tri;

	t_v3f	v0v1;
	t_v3f	v0v2;
	t_v3f	v1v2;

	t_v3f	v1p;
	t_v3f	v2p;

	t_v3f	n;
	float	area2;

	float	w;
	float	u;
	float	v;

	tri = &obj->tri;

	t_v2f	vt[3];
	generate_uv_vertices(vt, &obj->tri, obj->t.dir);

	// (p - v0) = u * v0v1 + v * v0v2
	// (v0p) = u * v0v1 + v * v0v2

	// parallelogram area = |(edge0) x (edge1)|
	//					  = |(edge0) x (edge1)| * .5f;
	// v0p = v3f_sub(world_point, tri->v0);
	v0v1 = v3f_sub(tri->v1, tri->v0);
	v0v2 = v3f_sub(tri->v2, tri->v0);
	v1v2 = v3f_sub(tri->v2, tri->v1);

	v1p = v3f_sub(world_point, tri->v1);
	v2p = v3f_sub(world_point, tri->v2);

	n = v3f_cross(v0v1, v0v2);
	area2 = 1.0f / v3f_sqr_mag(n);

	u = v3f_dot(n, v3f_cross(v1v2, v1p)) * area2;
	v = v3f_dot(n, v3f_cross(v3f_scale(v0v2, -1.0f), v2p)) * area2;
	w = 1.0f - u - v;

	t_v2f	coord;
	coord = init_v2f(
		u * vt[0].x + v * vt[1].x + w * vt[2].x,
		u * vt[0].y + v * vt[1].y + w * vt[2].y
	);
	return (coord);
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
