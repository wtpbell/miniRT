/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 18:40:12 by jboon             #+#    #+#             */
/*   Updated: 2025/06/25 11:45:59 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_H
# define VECTOR_H

# include <stdbool.h>
# include <math.h>

typedef union s_vector2
{
	struct
	{
		float	x;
		float	y;
	};
	struct
	{
		float	u;
		float	v;
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
	struct
	{
		float	u;
		float	v;
		float	theta;
	};
	float	_axis[3];
}	t_v3f;

extern const t_v3f	g_v3f_zero;
extern const t_v3f	g_v3f_one;
extern const t_v3f	g_v3f_foward;
extern const t_v3f	g_v3f_back;
extern const t_v3f	g_v3f_up;
extern const t_v3f	g_v3f_down;
extern const t_v3f	g_v3f_right;
extern const t_v3f	g_v3f_left;
extern const t_v2f	g_v2f_zero;
extern const t_v2f	g_v2f_one;

t_v2f	init_v2f(float x, float y);
t_v2f	v2f_rotate(t_v2f v, float angle);
t_v3f	init_v3f(float x, float y, float z);
t_v3f	v3f_add(t_v3f a, t_v3f b);
t_v3f	v3f_sub(t_v3f a, t_v3f b);
t_v3f	v3f_scale(t_v3f v, float f);
t_v3f	v3f_cross(t_v3f a, t_v3f b);
t_v3f	v3f_norm(t_v3f v);
t_v3f	v3f_refl(t_v3f d, t_v3f n);
t_v3f	v3f_refr(t_v3f uv, t_v3f n, float etai_over_etat);
t_v3f	v3f_lerp(t_v3f a, t_v3f b, float t);
t_v3f	v3f_mul(t_v3f a, t_v3f b);
t_v3f	v3f_clamp(t_v3f v, float min, float max);
t_v3f	v3f_clampf01(t_v3f v);
float	v3f_dot(t_v3f a, t_v3f b);
float	v3f_mag(t_v3f v);
float	v3f_sqr_mag(t_v3f v);
bool	v3f_equals(t_v3f a, t_v3f b, float epsilon);
t_v3f	v3f_sub_v2f(t_v3f a, t_v2f b);
t_v3f	v3f_mul_v2f(t_v3f a, t_v2f b);

#endif
