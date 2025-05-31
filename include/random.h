/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bell <bell@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 15:30:00 by bell              #+#    #+#             */
/*   Updated: 2023/05/31 15:30:00 by bell             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RANDOM_H
# define RANDOM_H

# include <stdint.h>
# include "vector.h"

// Generate a random point inside a unit sphere
t_v3f	random_in_unit_sphere(uint32_t *state);
float	random_float_pcg(uint32_t *state);
#endif
