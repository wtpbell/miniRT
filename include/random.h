/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 15:30:00 by bewong            #+#    #+#             */
/*   Updated: 2023/05/31 15:30:00 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RANDOM_H
# define RANDOM_H

# include <stdint.h>
# include "vector.h"

void		seed_rand(uint32_t seed);
uint32_t	get_rngstate(uint32_t x, uint32_t y, uint32_t frame);
float		frandom(void);
float		frandom_norm_distribution(void);
int			irand(int min, int max);
t_v3f		random_direction(void);
t_v3f		random_in_hemisphere(t_v3f normal);
t_v3f		frand_sphere(void);

#endif
