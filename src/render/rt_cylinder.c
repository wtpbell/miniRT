/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_cylinder.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/17 11:59:52 by bewong        #+#    #+#                 */
/*   Updated: 2025/05/20 12:29:46 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ray.h"
#include "scene.h"
#include "minirt.h"
#include "rt_math.h"

//https://math.stackexchange.com/questions/2613781/line-cylinder-intersection
// Oy + t *Dy = ydisc, t = (ydisc -Oy) / Dy

static t_v3f	cylinder_normal(t_obj *obj, t_v3f point)
{
	return (v3f_norm(v3f_sub(point, obj->t.pos)));
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
	if (*dst > FLT_EPI && *dst  < current)
	{
		p = v3f_add(ray->origin, v3f_scale(ray->direction, *dst));
		if ((p.x * p.x + p.z * p.z) <= r * r)
			return (1);
	}
	*dst = (h - ray->origin.y) / ray->direction.y;
	if (*dst > FLT_EPI && *dst  < current)
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
	float	t;

	if (!solve_quadratic(&coeff, &t_vals.x, &t_vals.y))
		return (0);
	i = 0;
	while (i < 2)
	{
		if (i == 0)
			t = t_vals.x;
		else
			t = t_vals.y;
		if (t > FLT_EPI)
		{
			p = v3f_add(ray->origin, v3f_scale(ray->direction, t));
			if (p.y >= -h / 2 && p.y <= h / 2)
				return (*dst = t, 1);
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
	t_mat4x4	mat[2];
	t_v2f		t_values;
	t_v2f		hits;

	obj_to_world(mat[0], obj->t.pos, obj->t.dir,
		(t_v3f){.x = 0, .y = 1, .z = 0});
	invert_m4x4(mat[1], mat[0]);
	l_ray.origin = mul_v3_m4x4(ray->origin, mat[1]);
	l_ray.direction = mul_dir_m4x4(ray->direction, mat[1]);
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
