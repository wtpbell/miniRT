/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lerp.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/24 12:16:21 by jboon         #+#    #+#                 */
/*   Updated: 2025/07/24 12:17:01 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "rt_math.h"

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
