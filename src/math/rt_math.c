/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_math.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/13 13:40:09 by jboon         #+#    #+#                 */
/*   Updated: 2025/07/24 12:18:36 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "rt_math.h"

int	fapprox(float n)
{
	return (fabsf(n) > FLT_SML);
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
