/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_math.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/13 13:40:09 by jboon         #+#    #+#                 */
/*   Updated: 2025/06/10 23:50:08 by jboon         ########   odam.nl         */
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

float	ft_clampf(float value, float min, float max)
{
	if (value < min)
		return (min);
	if (value > max)
		return (max);
	return (value);
}

inline float	ft_clampf01(float val)
{
	return (ft_clampf(val, 0.0f, 1.0f));
}

float	schlick(float cosin, float ref_idx)
{
	float	r0;

	r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return (r0 + (1 - r0) * powf(1 - cosin, 5));
}

inline float	modulo(const float x)
{
	return (x - floorf(x));
}
