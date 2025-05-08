/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   vector_helper.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/08 19:41:08 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/08 19:54:48 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>

#include "vector.h"

/**
 * @brief cosine of the angle between two vectors.
 *  0 means the vectors are orthogonal (perpendicular) to each other.
 * -1 means the vectors are going in opposing directions.
 *  1 means that both vectors are aligned in the same direction.
 */
inline float	v3f_dot(t_v3f a, t_v3f b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

/**
 * @brief
 * 	Ax   Bx	   Ay     By    a.y * b.z - a.z * b.y
 *  Ay x By => Az  x  Bz => a.z * b.x - a.x * b.z
 *  Az   Bz    Ax     Bx    a.x * b.y - a.y * b.x
 *             Ay     By
 */
inline t_v3f	v3f_cross(t_v3f a, t_v3f b)
{
	return ((t_v3f){
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	});
}

inline float	v3f_sqr_mag(t_v3f v)
{
	return (v3f_dot(v, v));
}

inline float	v3f_mag(t_v3f v)
{
	return (sqrtf(v3f_sqr_mag(v)));
}

t_v3f	v3f_norm(t_v3f v)
{
	float	len;

	len = v3f_sqr_mag(v);
	if (len > 0)
	{
		len = 1 / sqrtf(len);
		return (v3f_scale(v, len));
	}
	return (v);
}
