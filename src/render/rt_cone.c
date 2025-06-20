/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_cone.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 12:27:13 by bewong            #+#    #+#             */
/*   Updated: 2025/06/17 12:27:13 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene.h"
#include "rt_math.h"
#include "minirt.h"

/**
	f(x,y,z) = x² + z² - (k·y)² = 0
	normal = (x, -k²y, z) / √(x² + (k²y)² + z²)
	(x, -k²y, z) in local space
	larger k, the more the normal tilts outwards
**/
t_v3f	cone_normal(t_obj *obj, t_v3f point)
{
	t_v3f	obj_p;
	float	k_sq;

	obj_p = mul_v3_m4x4(point, obj->t.to_obj);
	if (fabsf(obj_p.y - obj->cone.height) < FLT_SML)
		return (v3f_norm(mul_dir_m4x4(g_v3f_up, obj->t.to_world)));
	k_sq = -(obj->cone.radius / obj->cone.height)
		* (obj->cone.radius / obj->cone.height);
	return (v3f_norm(mul_dir_m4x4(
			init_v3f(obj_p.x, k_sq * obj_p.y, obj_p.z),
			obj->t.to_world
		)));
}

static int check_cone_body(t_v3f coeff, t_ray *ray, float h, t_v2f *t)
{
	t_v3f	p;
	float	t0;
	float	t1;
	float	min_y;
	float	max_y;

	if (!solve_quadratic(&coeff, &t0, &t1))
		return (0);
	min_y = fminf(0.0f, h);
	max_y = fmaxf(0.0f, h);
	if (t0 > t->x && t0 < t->y)
	{
		p = v3f_add(ray->origin, v3f_scale(ray->direction, t0));
		if (p.y >= min_y && p.y <= max_y)
			return (t->y = t0, 1);
	}
	return (0);
}

/**
	clone equation: x² + z² = k² * r² = (y/h)² * r²
	P(t) = O + tD
	P(t) = ray->origin + t * ray->direction
	(Ox + tDx)² + (Oz + tDz)² = k² * (Oy + tDy)²
	(Dx² + Dz² - k²Dy²)t² + 2(OxDx + OzDz - k²OyDy)t + (Ox² + Oz² - k²Oy²) = 0
**/
static int	intersect_cone_body(t_obj *obj, t_ray *ray, t_v2f *t)
{
	t_v3f	coeff;
	float	k_sq;

	k_sq = (obj->cone.radius * obj->cone.radius)
		/ (obj->cone.height * obj->cone.height);
	coeff.x = ray->direction.x * ray->direction.x
		+ ray->direction.z * ray->direction.z
		- k_sq * ray->direction.y * ray->direction.y;
	coeff.y = 2 * (ray->origin.x * ray->direction.x
		+ ray->origin.z * ray->direction.z
		- k_sq * ray->origin.y * ray->direction.y);
	coeff.z = ray->origin.x * ray->origin.x
		+ ray->origin.z * ray->origin.z
		- k_sq * ray->origin.y * ray->origin.y;
	return (check_cone_body(coeff, ray, obj->cone.height, t));
}

int	cone_intersect(t_obj *obj, t_ray *ray, t_v2f t, float *dst)
{
	t_ray	l_ray;

	l_ray.origin = mul_v3_m4x4(ray->origin, obj->t.to_obj);
	l_ray.direction = mul_dir_m4x4(ray->direction, obj->t.to_obj);
	if ((intersect_cone_body(obj, &l_ray, &t)
		| intersect_disc(obj->cone.radius, obj->cone.height, &l_ray, &t)) == 1)
		return (*dst = t.y, 1);
	return (0);
}
