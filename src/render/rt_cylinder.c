/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_cylinder.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/17 11:59:52 by bewong        #+#    #+#                 */
/*   Updated: 2025/05/22 15:48:42 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ray.h"
#include "scene.h"
#include "rt_math.h"
#include "minirt.h"
#include <math.h>
#include <stdio.h>

t_v3f	cylinder_normal(t_obj *obj, t_v3f point)
{
	t_v3f	obj_p;

	obj_p = mul_v3_m4x4(point, obj->to_obj);
	if (fabsf(obj_p.y) >= obj->u_shape.cy.height * 0.5f - FLT_SML)
	{
		if (obj_p.y > 0)
			return (v3f_norm(mul_dir_m4x4(init_v3f(0, 1, 0), obj->to_world)));
		return (v3f_norm(mul_dir_m4x4(init_v3f(0, -1, 0), obj->to_world)));
	}
	return (v3f_norm(mul_dir_m4x4(init_v3f(obj_p.x, 0, obj_p.z),
				obj->to_world)));
}

static int	intersect_cylinder_discs(t_obj *obj, t_ray *ray,
	float *dst, float current)
{
	float	h;
	float	r;
	t_v3f	p;

	h = obj->u_shape.cy.height * 0.5f;
	r = obj->u_shape.cy.radius;
	if (ray->direction.y == 0)
		return (0);
	*dst = (-h - ray->origin.y) / ray->direction.y;
	if (*dst > FLT_SML && *dst < current)
	{
		p = v3f_add(ray->origin, v3f_scale(ray->direction, *dst));
		if ((p.x * p.x + p.z * p.z) <= r * r)
			return (1);
	}
	*dst = (h - ray->origin.y) / ray->direction.y;
	if (*dst > FLT_SML && *dst < current)
	{
		p = v3f_add(ray->origin, v3f_scale(ray->direction, *dst));
		if ((p.x * p.x + p.z * p.z) <= r * r)
			return (1);
	}
	return (0);
}

/*	x^2 + z^2 = r^2
	(Ox + t Dx)^2 + (Oz + t Dz)^2 = r^2
	(Ox^2 + t^2 Dx^2 + 2t Dx Ox) + (Oz^2 + t^2 Dz^2 + 2t Dz Oz) = r^2
	t^2 (Dx^2 + Dz^2) + 2t (Dx Ox + Dz Oz) + (Ox^2 + Oz^2 - r^2) = 0
	at^2 + bt + c = 0
*/
static int	check_body(t_v3f coeff, t_ray *ray, float h, float *dst)
{
	t_v2f	t_vals;
	t_v3f	p;
	int		i;

	if (!solve_quadratic(&coeff, &t_vals.x, &t_vals.y))
		return (0);
	i = 0;
	while (i < 2)
	{
		if (i == 0)
			*dst = t_vals.x;
		else
			*dst = t_vals.y;
		if (*dst > FLT_SML)
		{
			p = v3f_add(ray->origin, v3f_scale(ray->direction, *dst));
			if (p.y >= -h / 2 && p.y <= h / 2)
				return (1);
		}
		++i;
	}
	return (0);
}

static int	intersect_cylinder_body(t_obj *obj, t_ray *ray, float *dst)
{
	t_v3f	coeff;

	coeff.x = ray->direction.x * ray->direction.x
		+ ray->direction.z * ray->direction.z;
	coeff.y = 2 * (ray->origin.x * ray->direction.x
			+ ray->origin.z * ray->direction.z);
	coeff.z = ray->origin.x * ray->origin.x + ray->origin.z
		* ray->origin.z - obj->u_shape.cy.radius * obj->u_shape.cy.radius;
	return (check_body(coeff, ray, obj->u_shape.cy.height, dst));
}

int	cylinder_intersect(t_obj *obj, t_ray *ray, float *dst)
{
	t_ray		l_ray;
	t_v2f		t_values;
	t_v2f		hits;

	l_ray.origin = mul_v3_m4x4(ray->origin, obj->to_obj);
	l_ray.direction = mul_dir_m4x4(ray->direction, obj->to_obj);
	hits.x = intersect_cylinder_body(obj, &l_ray, &t_values.x);
	if (!hits.x)
		hits.y = intersect_cylinder_discs(obj, &l_ray, &t_values.y, FLT_MAX);
	else
		hits.y = intersect_cylinder_discs(obj, &l_ray, &t_values.y, t_values.x);
	if (hits.x && (!hits.y || t_values.x < t_values.y))
		return (*dst = t_values.x, 1);
	if (hits.y)
		return (*dst = t_values.y, 1);
	return (0);
}
