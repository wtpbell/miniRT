/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   vector_operation.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/08 19:38:24 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/22 13:52:08 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

inline t_v3f	v3f_add(t_v3f a, t_v3f b)
{
	return ((t_v3f){
		.x = a.x + b.x,
		.y = a.y + b.y,
		.z = a.z + b.z
	});
}

inline t_v3f	v3f_sub(t_v3f a, t_v3f b)
{
	return ((t_v3f){
		.x = a.x - b.x,
		.y = a.y - b.y,
		.z = a.z - b.z
	});
}

inline t_v3f	v3f_scale(t_v3f v, float f)
{
	return ((t_v3f){
		.x = v.x * f,
		.y = v.y * f,
		.z = v.z * f
	});
}

float	v3f_dist(t_v3f a, t_v3f b)
{
	return (v3f_mag(v3f_sub(a, b)));
}
