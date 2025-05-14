/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   vector_init.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/08 19:35:46 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/15 00:01:24 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

inline t_v2f	init_v2f(float x, float y)
{
	return ((t_v2f){
		.x = x,
		.y = y
	});
}

inline t_v3f	init_v3f(float x, float y, float z)
{
	return ((t_v3f){
		.x = x,
		.y = y,
		.z = z
	});
}
