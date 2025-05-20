/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_math.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/08 18:51:03 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/20 18:27:46 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_MATH_H
# define RT_MATH_H

# include <math.h>

# include "vector.h"
# include "matrix.h"

# define PI			3.14159265358979323846f
# define RADTODEG	57.2957795131f
# define DEGTORAD	0.01745329251f
# define FLT_MAX	3.40282347e+38f
# define FLT_EPI	1.19209290e-7f
# define FLT_SML	1e-5f

float	ft_maxf(float a, float b);
float	ft_signf(float a);
void	ft_swapf(float *a, float *b);
float	ft_absf(float a);

#endif
