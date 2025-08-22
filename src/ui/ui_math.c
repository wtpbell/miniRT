/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_math.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 17:54:31 by bewong            #+#    #+#             */
/*   Updated: 2025/08/20 17:54:31 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"

float	step_power_of_two(float value, float direction, t_v2f range)
{
	float	p;
	float	new_p;
	float	stepped;

	p = log2f(value);
	if (direction < 0)
		new_p = ceilf(p) - 1.0f;
	else
		new_p = floorf(p) + 1.0f;
	stepped = powf(2.0f, new_p);
	if (stepped < range.x)
		stepped = range.x;
	if (stepped > range.y)
		stepped = range.y;
	return (stepped);
}

float	step_linear(float value, float direction, float step, t_v2f range)
{
	float	stepped;

	stepped = value + direction * step;
	if (stepped < range.x)
		stepped = range.x;
	if (stepped > range.y)
		stepped = range.y;
	return (stepped);
}

uint32_t	blend_colors(uint32_t bg, uint32_t fg)
{
	float		alpha;
	float		inv_alpha;
	uint32_t	r;
	uint32_t	g;
	uint32_t	b;

	alpha = (fg & 0xFF) / 255.0f;
	inv_alpha = 1.0f - alpha;
	r = ((bg >> 24) & 0xFF) * inv_alpha + ((fg >> 24) & 0xFF) * alpha;
	g = ((bg >> 16) & 0xFF) * inv_alpha + ((fg >> 16) & 0xFF) * alpha;
	b = ((bg >> 8) & 0xFF) * inv_alpha + ((fg >> 8) & 0xFF) * alpha;
	return ((r << 24) | (g << 16) | (b << 8) | 0xFF);
}
