/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   vector.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/08 18:40:12 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/15 16:27:53 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_H
# define VECTOR_H

# include <stdbool.h>

typedef union s_vector2
{
	struct
	{
		float	x;
		float	y;
	};
	float	_axis[2];
}	t_v2f;

typedef union s_vector3
{
	struct
	{
		float	x;
		float	y;
		float	z;
	};
	float	_axis[3];
}	t_v3f;

t_v2f	init_v2f(float x, float y);
t_v3f	init_v3f(float x, float y, float z);
t_v3f	v3f_add(t_v3f a, t_v3f b);
t_v3f	v3f_sub(t_v3f a, t_v3f b);
t_v3f	v3f_scale(t_v3f v, float f);
t_v3f	v3f_cross(t_v3f a, t_v3f b);
t_v3f	v3f_norm(t_v3f v);
float	v3f_dot(t_v3f a, t_v3f b);
float	v3f_mag(t_v3f v);
float	v3f_sqr_mag(t_v3f v);

#endif
