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
