/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   vector_operations.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/01 18:58:07 by jboon         #+#    #+#                 */
/*   Updated: 2025/07/01 19:06:00 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "rt_math.h"
#include "vector.h"

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

	cos_theta = ft_clampf(v3f_dot(v3f_scale(uv, -1.0f), n), -1.0f, 1.0f);
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

t_v3f	v3f_sub_v2f(t_v3f a, t_v2f b)
{
	return (init_v3f(a.x - b.x, a.y - b.y, a.z));
}

t_v3f	v3f_mul_v2f(t_v3f a, t_v2f b)
{
	return (init_v3f(a.x * b.x, a.y * b.y, a.z));
}
