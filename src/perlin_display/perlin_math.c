/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_math.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 14:35:32 by bewong            #+#    #+#             */
/*   Updated: 2025/08/17 14:35:32 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "perlin_display.h"

void	delta_flt(t_val real, t_val delta, t_v2f range)
{
	float	value;

	value = *real.f + *delta.f;
	if (value < range.x)
		value = range.x;
	else if (value > range.y)
		value = range.y;
	*real.f = value;
}

void	delta_int(t_val real, t_val delta, t_v2f range)
{
	int	value;

	value = *real.i + *delta.i;
	if (value < (int)range.x)
		value = (int)range.x;
	else if (value > (int)range.y)
		value = (int)range.y;
	*real.i = value;
}

void	double_flt(t_val real, t_val ctx, t_v2f range)
{
	float	value;

	(void)ctx;
	value = *real.f * 2.0f;
	if (value < range.x)
		value = range.x;
	else if (value > range.y)
		value = range.y;
	*real.f = value;
}

void	half_flt(t_val real, t_val ctx, t_v2f range)
{
	float	value;

	(void)ctx;
	value = *real.f * 0.5f;
	if (value < range.x)
		value = range.x;
	else if (value > range.y)
		value = range.y;
	*real.f = value;
}
