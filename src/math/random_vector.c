/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   random_vector.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/30 19:08:47 by jboon         #+#    #+#                 */
/*   Updated: 2025/06/30 19:09:39 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "rt_math.h"
#include "random.h"

// Calculate a random direction
// source: Sebastion Lague
// [Random Hemisphere Directions]
// (https://www.youtube.com/watch?v=Qz0KTGYJtUk&t=674s)
// https://stackoverflow.com/a/6178290
t_v3f	random_direction(void)
{
	t_v3f	dir;

	dir.x = frandom_norm_distribution();
	dir.y = frandom_norm_distribution();
	dir.z = frandom_norm_distribution();
	return (v3f_norm(dir));
}

// random unit vector for diffuse reflection, create smooth no noise surface
t_v3f	random_in_hemisphere(t_v3f normal)
{
	t_v3f	v;
	float	len_sq;

	len_sq = 2.0f;
	while (len_sq >= 1.0f || len_sq == 0.0f)
	{
		v = init_v3f(
				frandom() * 2.0f - 1.0f,
				frandom() * 2.0f - 1.0f,
				frandom() * 2.0f - 1.0f
				);
		len_sq = v3f_dot(v, v);
	}
	if (v3f_dot(v, normal) > 0.0f)
		return (v3f_norm(v));
	else
		return (v3f_scale(v3f_norm(v), -1.0f));
}
