/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   noise_texturing.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/24 10:37:14 by jboon         #+#    #+#                 */
/*   Updated: 2025/07/31 16:24:21 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "rt_math.h"
#include "material.h"

static float	generate_perlin(t_v2f point, const t_perlin *data)
{
	int		curr_layer;
	float	sum;
	float	freq;
	float	ampt;

	curr_layer = 0;
	sum = 0.0f;
	freq = data->freq;
	ampt = data->ampt;
	while (curr_layer < data->layers)
	{
		sum += perlin(v2f_scale(point, freq)) * ampt;
		ampt *= data->gain;
		freq *= data->rate;
		++curr_layer;
	}
	return (sum);
}

float	pink_noise(t_v2f point, const t_perlin *pink)
{
	return (generate_perlin(point, pink));
}

float	turbulence_noise(t_v2f point, const t_perlin *turb)
{
	return (fabsf(generate_perlin(point, turb)));
}

float	marble_noise(t_v2f point, const t_perlin *marb)
{
	return (sin(
		(point.x + turbulence_noise(point, marb) * marb->marble.distortion)
			* marb->marble.scale));
}

float	wood_noise(t_v2f point, const t_perlin *wood)
{
	return (modulo(perlin(v2f_scale(point, wood->freq)) * wood->ampt));
}
