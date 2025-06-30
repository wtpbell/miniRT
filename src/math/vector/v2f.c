/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   v2f.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/30 20:06:59 by jboon         #+#    #+#                 */
/*   Updated: 2025/06/30 20:07:29 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

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
