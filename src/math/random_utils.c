/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bell <bell@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 15:30:00 by bell              #+#    #+#             */
/*   Updated: 2023/05/31 15:30:00 by bell             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "random.h"
#include "vector.h"

// PCG Random Number Generator
// LCG: state = (state * A + C) % 2^32, [prev state] --×747796405 +2891336453--> [new state]
// permutation step (*state >> 28) top 4, +4  shift between 4 and 19 bits, (*state >> shift_amount) extract high bits
// https://www.pcg-random.org/
// https://www.youtube.com/watch?v=45Oet5qjlms
inline float	random_float_pcg(uint32_t *state)
{
	uint32_t	result;

	*state = *state * 747796405 + 2891336453; // Linear Congruential Generator (LCG)
	result = ((*state >> ((*state >> 28) + 4)) ^ *state) * 277803737; // permutation step
	result = (result >> 22) ^ result;
	return ((float)result / 4294967295.0f); // normalized to [0, 1]
}


// Generate a random point inside a unit sphere
t_v3f	random_in_unit_sphere(uint32_t *state)
{
	t_v3f p;
	do {
		p.x = 2.0f * random_float_pcg(state) - 1.0f;
		p.y = 2.0f * random_float_pcg(state) - 1.0f;
		p.z = 2.0f * random_float_pcg(state) - 1.0f;
	} while (v3f_sqr_mag(p) >= 1.0f);
	return (p);
}
