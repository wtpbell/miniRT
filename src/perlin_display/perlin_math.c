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

void	delta_flt(t_val real, t_val delta)
{
	*real.f += *delta.f;
}

void	delta_int(t_val real, t_val delta)
{
	*real.i += *delta.i;
}

void	double_flt(t_val real, t_val ctx)
{
	(void)ctx;
	*real.f *= 2.0f;
}

void	half_flt(t_val real, t_val ctx)
{
	(void)ctx;
	*real.f *= 0.5f;
}
