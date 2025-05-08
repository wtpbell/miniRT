/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   vector_operation.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/08 19:38:24 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/08 19:55:23 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

inline t_v3f	v3f_add(t_v3f a, t_v3f b)
{
	return ((t_v3f){
		a.x + b.x,
		a.y + b.y,
		a.z + b.z
	});
}

inline t_v3f	v3f_sub(t_v3f a, t_v3f b)
{
	return ((t_v3f){
		a.x - b.x,
		a.y - b.y,
		a.z - b.z
	});
}

inline t_v3f	v3f_scale(t_v3f v, float f)
{
	return ((t_v3f){
		v.x * f,
		v.y * f,
		v.z * f
	});
}
