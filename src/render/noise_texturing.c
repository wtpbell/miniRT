/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   noise_texturing.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/24 10:37:14 by jboon         #+#    #+#                 */
/*   Updated: 2025/07/30 22:00:18 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "rt_math.h"
#include "material.h"

static float	generate_perlin(t_perlin *data, t_v2f point)
{
	int		curr_layer;
	float	sum;

	curr_layer = 0;
	sum = 0.0f;
	while (curr_layer < data->layers)
	{
		sum += perlin(v2f_scale(point, data->freq)) * data->ampt;
		data->ampt *= data->gain;
		data->freq *= data->rate;
		++curr_layer;
	}
	return (sum);
}

float	pink_noise(t_v2f point)
{
	t_perlin	pink;

	pink.rate = 2.0f;
	pink.gain = 0.5f;
	pink.freq = 1.0f;
	pink.ampt = 1.0f;
	pink.layers = 5;
	return (generate_perlin(&pink, point));
}

float	turbulence_noise(t_v2f point)
{
	t_perlin	turb;

	turb.rate = 1.8f;
	turb.gain = 0.35f;
	turb.freq = 0.02f;
	turb.ampt = 1.0f;
	turb.layers = 5;
	return (fabsf(generate_perlin(&turb, point)));
}

float	marble_noise(t_v2f point)
{
	return (sin((point.x + turbulence_noise(point) * 100.0f) * TAU / 200.0f));
}

float	wood_noise(t_v2f point)
{
	t_perlin	wood;

	wood.freq = 0.35f;
	wood.ampt = 10.0f;
	return (modulo(perlin(v2f_scale(point, wood.freq)) * wood.ampt));
}
