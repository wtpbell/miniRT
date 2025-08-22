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
