/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_math.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/13 13:40:09 by jboon         #+#    #+#                 */
/*   Updated: 2025/07/24 11:59:56 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "rt_math.h"

int	fapprox(float n)
{
	return (fabsf(n) > FLT_SML);
}

float	schlick(float cosin, float ref_idx)
{
	float	r0;

	r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return (r0 + (1 - r0) * powf(1 - cosin, 5));
}

float	modulo(float x)
{
	return (x - floorf(x));
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

void	ft_swapi(int *a, int *b)
{
	int	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

float	lerpf(float min, float max, float t)
{
	return (min * (1.0f - t) + max * t);
}

float	cos_smooth(float t)
{
	return ((1.0f - cosf(t * PI)) * .5f);
}

float	perlin_smoothstep(float t)
{
	return (t * t * t * (t * (t * 6.0f - 15.0f) + 10.0));
}

float	smoothstep(float t)
{
	return (t * t * (3 - 2 * t));
}
