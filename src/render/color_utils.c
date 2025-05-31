/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 16:05:00 by cascade          #+#    #+#             */
/*   Updated: 2025/05/28 16:05:00 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "color.h"
#include "vector.h"
#include "rt_math.h"

//before multiplying color by 255
t_v3f	col32_to_v3f(t_col32 color)
{
	return ((t_v3f){
		.x = get_r(color) / 255.0f,
		.y = get_g(color) / 255.0f,
		.z = get_b(color) / 255.0f
	});
}

//for displaying final color
t_col32	v3f_to_col32(t_v3f color)
{
	return (init_col32(
			(int)(ft_clampf(color.x, 0.0f, 1.0f) * 255.0f),
			(int)(ft_clampf(color.y, 0.0f, 1.0f) * 255.0f),
			(int)(ft_clampf(color.z, 0.0f, 1.0f) * 255.0f),
			255
	));
}

//linear interpolation between two colors, for color fading
t_col32	lerp_col32(t_col32 a, t_col32 b, float t)
{
	return (v3f_to_col32(v3f_lerp(col32_to_v3f(a), col32_to_v3f(b), t)));
}

//add colors with clamped to [0,1]
t_col32	add_colors_clamped(t_col32 a, t_col32 b)
{
	return (v3f_to_col32(v3f_add(col32_to_v3f(a), col32_to_v3f(b))));
}

// color correction
t_col32	apply_gamma(t_col32 color, float gamma)
{
	t_v3f	v;
	float	inv_gamma;

	v = col32_to_v3f(color);
	inv_gamma = 1.0f / gamma;
	return (v3f_to_col32((t_v3f){
		.x = fminf(powf(v.x, inv_gamma), 1.0f),
		.y = fminf(powf(v.y, inv_gamma), 1.0f),
		.z = fminf(powf(v.z, inv_gamma), 1.0f)
	}));
}

t_col32	col32_scale(t_col32 color, float factor)
{
	return (v3f_to_col32(v3f_scale(col32_to_v3f(color), factor)));
}

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
