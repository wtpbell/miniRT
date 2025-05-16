/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_math.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/13 13:40:09 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/15 15:09:34 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

float	ft_maxf(float a, float b)
{
	if (a > b)
		return (a);
	return (b);
}

float	ft_signf(float a)
{
	if (a < 0)
		return (-1.0f);
	return (1.0f);
}

void	ft_swapf(float *a, float *b)
{
	float	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

float	ft_absf(float a)
{
	if (a < 0)
		return (-a);
	return (a);
}
