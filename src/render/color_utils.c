/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 16:05:00 by bewong          #+#    #+#             */
/*   Updated: 2025/06/01 19:31:00 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "color.h"
#include "vector.h"
#include "rt_math.h"

t_v3f	col32_to_v3f(t_col32 color)
{
	return ((t_v3f){{
			.x = get_r(color) / 255.0f,
			.y = get_g(color) / 255.0f,
			.z = get_b(color) / 255.0f
		}});
}

t_col32	v3f_to_col32(t_v3f color)
{
	return (init_col32(
			(int)(ft_clampf(color.x, 0.0f, 1.0f) * 255.0f),
		(int)(ft_clampf(color.y, 0.0f, 1.0f) * 255.0f),
		(int)(ft_clampf(color.z, 0.0f, 1.0f) * 255.0f),
		255
	));
}

// Linear interpolation
t_col32	col32_lerp(t_col32 x, t_col32 y, float t)
{
	uint32_t	r;
	uint32_t	g;
	uint32_t	b;
	uint32_t	alpha;

	if (t <= 0.0f)
		return (x);
	if (t >= 1.0f)
		return (y);
	r = (uint32_t)(get_r(x) * (1.0f - t) + get_r(y) * t);
	g = (uint32_t)(get_g(x) * (1.0f - t) + get_g(y) * t);
	b = (uint32_t)(get_b(x) * (1.0f - t) + get_b(y) * t);
	alpha = (uint32_t)(get_a(x) * (1.0f - t) + get_a(y) * t);
	return (init_col32(r, g, b, alpha));
}

t_v3f	v3f_apply_gamma(t_v3f color, float gamma)
{
	float	inv_gamma;

	inv_gamma = 1.0f / gamma;
	return ((t_v3f){{
			.x = fminf(powf(color.x, inv_gamma), 1.0f),
			.y = fminf(powf(color.y, inv_gamma), 1.0f),
			.z = fminf(powf(color.z, inv_gamma), 1.0f)
		}});
}
