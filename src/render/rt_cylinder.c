/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_cylinder.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/17 11:59:52 by bewong        #+#    #+#                 */
/*   Updated: 2025/05/27 17:52:59 by jboon         ########   odam.nl         */
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

	obj_p = mul_v3_m4x4(point, obj->t.to_obj);
	if (fabsf(obj_p.y) >= obj->u_shape.cy.height * 0.5f - FLT_SML)
	{
		if (obj_p.y > 0)
			return (v3f_norm(mul_dir_m4x4(init_v3f(0, 1, 0), obj->t.to_world)));
		return (v3f_norm(mul_dir_m4x4(init_v3f(0, -1, 0), obj->t.to_world)));
	}
	return (v3f_norm(mul_dir_m4x4(init_v3f(obj_p.x, 0, obj_p.z),
				obj->t.to_world)));
}

static int	intersect_cylinder_disc(float r, float h, t_ray *ray, t_v2f *t_lim)
{
	float	t;
	t_v3f	p;

	if (fabsf(ray->direction.y) < FLT_SML)
		return (0);
	t = (h - ray->origin.y) / ray->direction.y;
	if (t > t_lim->x && t < t_lim->y)
	{
		p = v3f_add(ray->origin, v3f_scale(ray->direction, t));
		if ((p.x * p.x + p.z * p.z) <= r * r)
			return (t_lim->y = t, 1);
	}
	return (0);
}

static int	check_body(t_v3f coeff, t_ray *ray, float h, t_v2f *t)
{
	t_v3f	p;
	float	t0;
	float	t1;

	if (!solve_quadratic(&coeff, &t0, &t1))
		return (0);
	if (t0 > t->x && t0 < t->y)
	{
		p = v3f_add(ray->origin, v3f_scale(ray->direction, t0));
		if (p.y >= -h * .5f && p.y <= h * .5f)
			return (t->y = t0, 1);
	}
	return (0);
}

/*
	x^2 + z^2 = r^2
	(Ox + t Dx)^2 + (Oz + t Dz)^2 = r^2
	(Ox^2 + t^2 Dx^2 + 2t Dx Ox) + (Oz^2 + t^2 Dz^2 + 2t Dz Oz) = r^2
	t^2 (Dx^2 + Dz^2) + 2t (Dx Ox + Dz Oz) + (Ox^2 + Oz^2 - r^2) = 0
	at^2 + bt + c = 0
*/
static int	intersect_cylinder_body(t_obj *obj, t_ray *ray, t_v2f *t)
{
	t_v3f	coeff;

	coeff.x = ray->direction.x * ray->direction.x
		+ ray->direction.z * ray->direction.z;
	coeff.y = 2 * (ray->origin.x * ray->direction.x
			+ ray->origin.z * ray->direction.z);
	coeff.z = ray->origin.x * ray->origin.x + ray->origin.z
		* ray->origin.z - obj->u_shape.cy.radius * obj->u_shape.cy.radius;
	return (check_body(coeff, ray, obj->u_shape.cy.height, t));
}

int	cylinder_intersect(t_obj *obj, t_ray *ray, t_v2f t, float *dst)
{
	t_ray	l_ray;
	float	r;
	float	h;

	r = obj->u_shape.cy.radius;
	h = obj->u_shape.cy.height * .5f;
	l_ray.origin = mul_v3_m4x4(ray->origin, obj->t.to_obj);
	l_ray.direction = mul_dir_m4x4(ray->direction, obj->t.to_obj);
	if ((intersect_cylinder_body(obj, &l_ray, &t)
			| intersect_cylinder_disc(r, h, &l_ray, &t)
			| intersect_cylinder_disc(r, -h, &l_ray, &t)) == 1)
		return (*dst = t.y, 1);
	return (0);
}
