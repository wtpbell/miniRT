/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_math.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 18:51:03 by jboon             #+#    #+#             */
/*   Updated: 2025/08/14 20:21:51 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_MATH_H
# define RT_MATH_H

# include <math.h>
# include "vector.h"
# include "matrix.h"

# define PI			3.14159265358979323846f
# define TAU		6.28318530717958647692f
# define RADTODEG	57.2957795131f
# define DEGTORAD	0.01745329251f
# define FLT_MAX	3.40282347e+38f
# define FLT_EPI	1.19209290e-7f
# define FLT_SML	1e-5f
# define BIAS		1e-2f
# define GAMMA		2.2f

float	ft_maxf(float a, float b);
float	ft_signf(float a);
void	ft_swapf(float *a, float *b);
void	ft_swapi(int *a, int *b);
int		fapprox(float n);
float	ft_clampf(float value, float min, float max);
float	ft_clampf01(float val);
float	modulo(float x);
float	lerpf(float min, float max, float t);
float	cos_smooth(float t);
float	perlin_smoothstep(float t);
float	smoothstep(float t);
void	init_perlin(void);
float	perlin(t_v2f point);

#endif
