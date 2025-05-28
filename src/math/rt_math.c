/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_math.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 13:40:09 by jboon             #+#    #+#             */
/*   Updated: 2025/05/28 16:08:33 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_math.h"

float	ft_maxf(float a, float b)
{
	if (a > b)
		return (a);
	return (b);
}

float	ft_signf(float a)
{
	if (a < 0)
		return (-1.0f);
	return (1.0f);
}

void	ft_swapf(float *a, float *b)
{
	float	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

inline int	fapprox(float n)
{
	return (fabsf(n) > FLT_SML);
}

int	ft_min(int a, int b)
{
	if (a < b)
		return (a);
	return (b);
}

float	ft_clampf(float value, float min, float max)
{
	if (value < min)
		return (min);
	if (value > max)
		return (max);
	return (value);
}
