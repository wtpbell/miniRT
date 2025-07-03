/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   vector_operation.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/08 19:38:24 by jboon         #+#    #+#                 */
/*   Updated: 2025/07/01 18:58:35 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "rt_math.h"
#include "vector.h"

t_v3f	v3f_add(t_v3f a, t_v3f b)
{
	return ((t_v3f){
		.x = a.x + b.x,
		.y = a.y + b.y,
		.z = a.z + b.z
	});
}

t_v3f	v3f_sub(t_v3f a, t_v3f b)
{
	return ((t_v3f){
		.x = a.x - b.x,
		.y = a.y - b.y,
		.z = a.z - b.z
	});
}

t_v3f	v3f_scale(t_v3f v, float f)
{
	return ((t_v3f){
		.x = v.x * f,
		.y = v.y * f,
		.z = v.z * f
	});
}

t_v3f	v3f_mul(t_v3f a, t_v3f b)
{
	return ((t_v3f){
		.x = a.x * b.x,
		.y = a.y * b.y,
		.z = a.z * b.z
	});
}

bool	v3f_equals(t_v3f a, t_v3f b, float epsilon)
{
	return (fabsf(a.x - b.x) < epsilon
		&& fabsf(a.y - b.y) < epsilon
		&& fabsf(a.z - b.z) < epsilon);
}
