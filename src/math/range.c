/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   range.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/30 19:28:01 by jboon         #+#    #+#                 */
/*   Updated: 2025/06/30 19:50:20 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

float	ft_maxf(float a, float b)
{
	if (a > b)
		return (a);
	return (b);
}

float	ft_clampf(float value, float min, float max)
{
	if (value < min)
		return (min);
	if (value > max)
		return (max);
	return (value);
}

extern inline float	ft_clampf01(float val)
{
	return (ft_clampf(val, 0.0f, 1.0f));
}
