/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   noise_texturing.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/24 10:37:14 by jboon         #+#    #+#                 */
/*   Updated: 2025/07/24 12:13:50 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "rt_math.h"
#include "scene.h"

float	pink_noise(t_v2f point)
{
	const float	rate = 2.0;
	const float	gain = 0.5f;
	const int	max_layer = 5;
	float		freq = 1.0f;
	float		ampt = 1.0f;
	float		sum;
	int			layer;

	sum = 0.0f;
	layer = 0;
	point = v2f_scale(point, freq);
	while (layer < max_layer)
	{
		sum += perlin(point) * ampt;
		point = v2f_scale(point, rate);
		ampt *= gain;
		++layer;
	}
	return (sum);
}

float	marble_noise(t_v2f point)
{
	const float	rate = 1.8f;
	const float	gain = .35f;
	const int	max_layer = 5;
	float		freq = 0.02f;
	float		ampt = 1.0f;
	float		sum;
	float		x;
	int			layer;

	sum = 0.0f;
	layer = 0;
	x = point.x;
	point = v2f_scale(point, freq);
	while (layer < max_layer)
	{
		sum += perlin(point) * ampt;
		point = v2f_scale(point, rate);
		ampt *= gain;
		++layer;
	}
	return ((sin((x + sum * 100.0f) * TAU / 200.0f) + 1.0f) * 0.5f);
}

float	wood_noise(t_v2f point)
{
	const float	freq = 0.35f;
	float		val;

	val = perlin(v2f_scale(point, freq)) * 10.0f;
	return (modulo(val));
}

float	turbulence_noise(t_v2f point)
{
	const float	rate = 1.8;
	const float	gain = 0.35f;
	const int	max_layer = 5;
	float		freq = 0.02f;
	float		ampt = 1.0f;
	float		sum;
	int			layer;

	sum = 0.0f;
	layer = 0;
	point = v2f_scale(point, freq);
	while (layer < max_layer)
	{
		sum += fabsf(2.0f * perlin(point) - 1.0f) * ampt;
		point = v2f_scale(point, rate);
		ampt *= gain;
		++layer;
	}
	return (sum);
}

inline void	fract(t_v2f v)
{
	v.x = modulo(v.x);
	v.y = modulo(v.y);
}

// spectral densities - to describe the various frequencies (layers) of the resulting noise is made of
// power spectra - each layer has a specific amplitude
// octave - can be misleading as an octave can describe the doubling or halving of a frequency of each layer
t_v3f	noise_pattern(const t_v2f *texcoord, const t_tex *tex, t_v3f col_a)
{
	t_v2f	point;

	point = v2f_mul_v3f(*texcoord, tex->scale_rot);
	fract(point);
	return (v3f_lerp(col_a, tex->col, (pink_noise(point) + 1.0f) * 0.5f));
}
