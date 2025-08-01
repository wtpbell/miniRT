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
#include "rt_thread.h"

static uint32_t	g_state[THRD_CNT];

void	seed_rand(uint32_t seed)
{
	g_state[get_thread_index()] = seed;
}

uint32_t	get_rngstate(uint32_t x, uint32_t y, uint32_t frame)
{
	uint32_t	hash;

	hash = (x * 0x1f1f1f1f) ^ y;
	return (hash + frame * 0x9e3779b9);
}

// PCG Random Number Generator
// https://www.pcg-random.org/
// https://www.youtube.com/watch?v=45Oet5qjlms
float	frandom(void)
{
	uint32_t	result;
	int			thrd_i;

	thrd_i = get_thread_index();
	g_state[thrd_i] = g_state[thrd_i] * 747796405 + 2891336453;
	result = ((g_state[thrd_i] >> ((g_state[thrd_i] >> 28) + 4))
			^ g_state[thrd_i]) * 277803737;
	result = (result >> 22) ^ result;
	return ((float)result / 4294967295.0f);
}

// Random value in normal distribution (with mean=0 and sd=1)
// https://stackoverflow.com/a/6178290
// https://en.wikipedia.org/wiki/Box-Muller_transform
// https://www.youtube.com/watch?v=Qz0KTGYJtUk&t=674s
float	frandom_norm_distribution(void)
{
	return (sqrtf(-2.0f * logf(frandom()))
		* cosf(2.0f * M_PI * frandom()));
}
