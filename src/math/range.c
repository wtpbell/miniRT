/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   range.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/30 19:28:01 by jboon         #+#    #+#                 */
/*   Updated: 2025/07/01 18:55:38 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

float	ft_maxf(float a, float b)
{
	if (a > b)
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

float	ft_clampf01(float val)
{
	return (ft_clampf(val, 0.0f, 1.0f));
}

t_v3f	v3f_clamp(t_v3f v, float min, float max)
{
	return ((t_v3f){{
			.x = ft_clampf(v.x, min, max),
			.y = ft_clampf(v.y, min, max),
			.z = ft_clampf(v.z, min, max)
		}});
}

t_v3f	v3f_clampf01(t_v3f v)
{
	return (v3f_clamp(v, 0.0f, 1.0f));
}
