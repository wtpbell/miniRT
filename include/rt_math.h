/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_math.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/08 18:51:03 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/16 18:52:32 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_MATH_H
# define RT_MATH_H

# include <math.h>

# include "vector.h"
# include "matrix.h"

# define PI			3.14159265358979323846f
# define RADTODEG	57.2957795131f
# define DEGTORAD	0.01745329251
# define FLT_MAX	3.40282347e+38F
# define FLT_EPI	1.19209290e-7F

float	ft_maxf(float a, float b);
float	ft_signf(float a);
void	ft_swapf(float *a, float *b);
float	ft_absf(float a);

#endif
