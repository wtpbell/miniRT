/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   v2f_helper.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/30 15:27:34 by jboon         #+#    #+#                 */
/*   Updated: 2025/07/30 15:36:19 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "rt_math.h"

t_v2f	v2f_fract(t_v2f v)
{
	return (init_v2f(
			modulo(v.x),
			modulo(v.y)
		));
}

t_v2f	v2f_mul_v3f(t_v2f a, t_v3f b)
{
	return (init_v2f(a.x * b.x, a.y * b.y));
}
