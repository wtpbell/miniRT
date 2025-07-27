/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   v2f.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/30 20:06:59 by jboon         #+#    #+#                 */
/*   Updated: 2025/07/27 23:26:22 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"
#include "rt_math.h"

t_v2f	v2f_scale(t_v2f v, float f)
{
	return (init_v2f(
		v.x * f,
		v.y * f
	));
}

t_v2f	v2f_mul(t_v2f a, t_v2f b)
{
	return (init_v2f(
		a.x * b.x,
		a.y * b.y
	));
}

void	v2f_fract(t_v2f *v)
{
	v->x = modulo(v->x);
	v->y = modulo(v->y);
}

float	v2f_mag(t_v2f v)
{
	return (sqrt(v.x * v.x + v.y * v.y));
}

t_v2f	v2f_rotate(t_v2f v, float angle)
{
	return (init_v2f(v.x * cosf(angle) - v.y * sinf(angle),
			v.y * cosf(angle) + v.x * sinf(angle))
	);
}

t_v2f	v2f_mul_v3f(t_v2f a, t_v3f b)
{
	return (init_v2f(a.x * b.x, a.y * b.y));
}
