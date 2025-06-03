/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 15:30:00 by bewong            #+#    #+#             */
/*   Updated: 2023/05/31 15:30:00 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <stdint.h>
#include "vector.h"

static uint32_t	g_state;

void	seed_rand(uint32_t seed)
{
	g_state = seed;
}

// x * large prime number in hex, ^ y to mix x and y
// + frame * large prime number in hex to mix frame
// 0x9e3779b9 golden ratio prime
uint32_t	get_rngstate(uint32_t x, uint32_t y, uint32_t frame)
{
	uint32_t	hash;

	hash = (x * 0x1f1f1f1f) ^ y;
	return (hash + frame * 0x9e3779b9);
}

// PCG Random Number Generator
// LCG: state = (state * A + C) % 2^32, [prev state] --×747796405 +2891336453--> [new state]
// permutation step (*state >> 28) top 4, +4  shift between 4 and 19 bits, (*state >> shift_amount) extract high bits
// https://www.pcg-random.org/
// https://www.youtube.com/watch?v=45Oet5qjlms
float	frandom(void)
{
	uint32_t	result;

	g_state = g_state * 747796405 + 2891336453; // Linear Congruential Generator (LCG)
	result = ((g_state >> ((g_state >> 28) + 4)) ^ g_state) * 277803737; // permutation step
	result = (result >> 22) ^ result;
	return ((float)result / 4294967295.0f); // normalized to [0, 1]
}

// Random value in normal distribution (with mean=0 and sd=1) ()
// https://stackoverflow.com/a/6178290
// https://en.wikipedia.org/wiki/Box-Muller_transform
// https://www.youtube.com/watch?v=Qz0KTGYJtUk&t=674s
float	frandom_norm_distribution(void)
{
	return (sqrtf(-2.0f * logf(frandom()))
		* cosf(2.0f * M_PI * frandom()));
}

// Calculate a random direction
// source: Sebastion Lague
// [Random Hemisphere Directions]
// (https://www.youtube.com/watch?v=Qz0KTGYJtUk&t=674s)
// https://stackoverflow.com/a/6178290
t_v3f	random_direction(void)
{
	t_v3f	dir;

	dir.x = frandom_norm_distribution();
	dir.y = frandom_norm_distribution();
	dir.z = frandom_norm_distribution();
	return (v3f_norm(dir));
}
