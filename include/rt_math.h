/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_math.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/08 18:51:03 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/13 17:36:30 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATH_H
# define MATH_H

# include <float.h>
# include <math.h>

# include "vector.h"

# define PI 3.14159265358979323846f
# define RADTODEG 57.2957795131f
# define DEGTORAD 0.01745329251

float	ft_maxf(float a, float b);
float	ft_signf(float a);
void	ft_swapf(float *a, float *b);

#endif
