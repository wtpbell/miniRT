/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   vector_operation.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/08 19:38:24 by jboon         #+#    #+#                 */
/*   Updated: 2025/06/18 17:01:16 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "rt_math.h"
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

// Reflection formula: r = d - 2*(d·n)*n
t_v3f	v3f_refl(t_v3f d, t_v3f n)
{
	return (v3f_sub(d, v3f_scale(n, 2 * v3f_dot(d, n))));
}

t_v3f	v3f_refr(t_v3f uv, t_v3f n, float etai_over_etat)
{
	float	cos_theta;
	t_v3f	r_out_perp;
	t_v3f	r_out_parallel;

	cos_theta = fminf(v3f_dot(v3f_scale(uv, -1.0f), n), 1.0f);
	r_out_perp = v3f_scale(v3f_add(uv, v3f_scale(n, cos_theta)),
			etai_over_etat);
	r_out_parallel = v3f_scale(n, -sqrtf(fabsf(
					1.0f - v3f_dot(r_out_perp, r_out_perp))));
	return (v3f_add(r_out_perp, r_out_parallel));
}

t_v3f	v3f_lerp(t_v3f a, t_v3f b, float t)
{
	return ((t_v3f){
		.x = a.x + (b.x - a.x) * t,
		.y = a.y + (b.y - a.y) * t,
		.z = a.z + (b.z - a.z) * t
	});
}

inline t_v3f	v3f_clamp(t_v3f v, float min, float max)
{
	return ((t_v3f){{
			.x = ft_clampf(v.x, min, max),
			.y = ft_clampf(v.y, min, max),
			.z = ft_clampf(v.z, min, max)
		}});
}

inline t_v3f	v3f_clampf01(t_v3f v)
{
	return (v3f_clamp(v, 0.0f, 1.0f));
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

t_v2f	v2f_rotate(t_v2f v, float angle)
{
	return (init_v2f(v.x * cosf(angle) - v.y * sinf(angle),
			v.y * cosf(angle) + v.x * sinf(angle))
	);
}

t_v3f	v3f_sub_v2f(t_v3f a, t_v2f b)
{
	return (init_v3f(a.x - b.x, a.y - b.y, a.z));
}

t_v3f	v3f_mul_v2f(t_v3f a, t_v2f b)
{
	return (init_v3f(a.x * b.x, a.y * b.y, a.z));
}
