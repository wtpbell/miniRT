/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_cylinder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 11:59:52 by bewong            #+#    #+#             */
/*   Updated: 2025/05/18 21:25:13 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ray.h"
#include "scene.h"
#include "minirt.h"
#include "rt_math.h"

//https://math.stackexchange.com/questions/2613781/line-cylinder-intersection
// Oy + t *Dy = ydisc, t = (ydisc -Oy) / Dy

static int	check_disc(t_ray *ray, float y, float r, float *dst, float current)
{
	float	t;
	t_v3f	p;

	t = (y - ray->origin.y) / ray->direction.y; //along y axis
	if (t < 0.001f || t > current)
		return (0);
	p = v3f_add(ray->origin, v3f_scale(ray->direction, t));
	if ((p.x * p.x + p.z * p.z) <= r * r) //check if pt is within the radius of the disc
	{
		*dst = t;
		return (1);
	}
	return (0);
}

static int	intersect_cylinder_discs(t_obj *obj, t_ray *ray, float *dst, float current)
{
	float	h;
	float	r;

	h = obj->u_shape.cy.height * 0.5f;
	r = obj->u_shape.cy.radius;
	if (ray->direction.y != 0) // if ray is not parallel to the disc
	{
		if (check_disc(ray, -h, r, dst, current))
			return (1);
		if (check_disc(ray, h, r, dst, current))
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
static int	check_body(t_v3f coeff, t_v3f o, t_v3f d, float h, float *dst)
{
	float	t0;
	float	t1;
	t_v3f	p;
	int		i;
	float	t;

	if (!solve_quadratic(&coeff, &t0, &t1))
		return (0);
	i = 0;
	while (i < 2)
	{
		if (i == 0)
			t = t0;
		else
			t = t1;
		if (t > 0.001f)
		{
			p = v3f_add(o, v3f_scale(d, t)); //pt along ray
			if (p.y >= -h / 2 && p.y <= h / 2) // check if inside cylinder height bounds
			{
				*dst = t;
				return (1);
			}
		}
		++i;
	}
	return (0);
}

static int	intersect_cylinder_body(t_obj *obj, t_ray *ray, float *dst)
{
	t_v3f	o;
	t_v3f	d;
	t_v3f	coeff;

	o = ray->origin;
	d = ray->direction;
	coeff.x = d.x * d.x + d.z * d.z;
	coeff.y = 2 * (o.x * d.x + o.z * d.z);
	coeff.z = o.x * o.x + o.z * o.z - obj->u_shape.cy.radius * obj->u_shape.cy.radius;
	return (check_body(coeff, o, d, obj->u_shape.cy.height, dst));
}

int	cylinder_intersect(t_obj *obj, t_ray *ray, float *dst)
{
	t_ray		l_ray;
	t_mat4x4	to_world;
	t_mat4x4	to_obj;
	float		t_body;
	float		t_disc;
	int			hit_body;
	int			hit_disc;

	obj_to_world(to_world, obj->t.pos, obj->t.dir);
	invert_m4x4(to_obj, to_world);
	l_ray.origin = mul_v3_m4x4(ray->origin, to_obj);
	l_ray.direction = mul_dir_m4x4(ray->direction, to_obj);
	hit_body = intersect_cylinder_body(obj, &l_ray, &t_body);
	if (!hit_body)
		hit_disc = intersect_cylinder_discs(obj, &l_ray, &t_disc, FLT_MAX);
	else
		hit_disc = intersect_cylinder_discs(obj, &l_ray, &t_disc, t_body);
	if (hit_body && (!hit_disc || t_body < t_disc))
	{
		*dst = t_body;
		return (1);
	}
	if (hit_disc)
	{
		*dst = t_disc;
		return (1);
	}
	return (0);
}

