/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_triangle.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/29 14:00:37 by jboon         #+#    #+#                 */
/*   Updated: 2025/06/10 10:26:41 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ray.h"
#include "rt_math.h"
#include "scene.h"

// TODO: normal should be be based on the side (front/back) it hits of the
//		 triangle. Now it always return the 'outside' normal.
t_v3f	triangle_normal(t_obj *obj, t_v3f point)
{
	(void)point;
	return (obj->t.dir);
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
	if (u < 0 || u > 1)
		return (0);
	qvec = v3f_cross(tvec, vars->v0v1);
	v = v3f_dot(ray->direction, qvec) * vars->det;
	if (v < 0 || (u + v) > 1.0f)
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
