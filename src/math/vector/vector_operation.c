/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_operation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:38:24 by jboon             #+#    #+#             */
/*   Updated: 2025/05/31 23:44:13 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"
#include <stdio.h>

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

inline t_v3f	v3f_neg(t_v3f v)
{
	return (v3f_sub(init_v3f(0, 0, 0), v));
}

bool	v3f_near_zero(t_v3f v)
{
	return (fabsf(v.x) < FLT_EPSILON && fabsf(v.y) < FLT_EPSILON
		&& fabsf(v.z) < FLT_EPSILON);
}

t_v3f	v3f_unit(t_v3f v)
{
	return (v3f_scale(v, 1.0f / v3f_mag(v)));
}

t_v3f	v3f_refl(t_v3f d, t_v3f n)
{
	// Reflection formula: r = d - 2*(d·n)*n
	return (v3f_sub(d, v3f_scale(n, 2 * v3f_dot(d, n))));
}

t_v3f	v3f_refr(t_v3f uv, t_v3f n, float etai_over_etat)
{
	float	cos_theta;
	t_v3f	r_out_perp;
	t_v3f	r_out_parallel;

	cos_theta = fminf(v3f_dot(v3f_neg(uv), n), 1.0f);
	r_out_perp = v3f_scale(v3f_add(uv, v3f_scale(n, cos_theta))
			, etai_over_etat);
	r_out_parallel = v3f_scale(n, -sqrtf(fabsf(
					1.0f - v3f_dot(r_out_perp, r_out_perp))));
	return (v3f_add(r_out_perp, r_out_parallel));
}

float	v3f_schlick(float cosin, float ref_idx)
{
	float	r0;

	r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return (r0 + (1 - r0) * powf(1 - cosin, 5));
}

t_v3f	v3f_lerp(t_v3f a, t_v3f b, float t)
{
	return ((t_v3f){
		.x = a.x + (b.x - a.x) * t,
		.y = a.y + (b.y - a.y) * t,
		.z = a.z + (b.z - a.z) * t
	});
}
