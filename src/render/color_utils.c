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

#include "MLX42/MLX42_Int.h"
#include "color.h"
#include "vector.h"
#include "rt_math.h"
#include "ray.h"

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
	return (init_col32((int)(ft_clampf01(color.x) * 255.0f),
		(int)(ft_clampf01(color.y) * 255.0f),
		(int)(ft_clampf01(color.z) * 255.0f),
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
			.x = ft_clampf(powf(color.x, inv_gamma), 0.0f, 1.0f),
			.y = ft_clampf(powf(color.y, inv_gamma), 0.0f, 1.0f),
			.z = ft_clampf(powf(color.z, inv_gamma), 0.0f, 1.0f)
		}});
}

uint32_t	rt_get_pixel(mlx_image_t *img, uint32_t x, uint32_t y)
{
	uint8_t	*pixel;

	pixel = img->pixels + (y * img->width + x) * BPP;
	return (((uint32_t)pixel[0] << 24)
		| ((uint32_t)pixel[1] << 16)
		| ((uint32_t)pixel[2] << 8)
		| (uint32_t)pixel[3]);
}
