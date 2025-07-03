/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   color.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/09 16:17:13 by jboon         #+#    #+#                 */
/*   Updated: 2025/07/01 18:53:23 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "color.h"

t_col32	init_col32(uint32_t r, uint32_t g, uint32_t b, uint32_t a)
{
	return ((r << 24) | (g << 16) | (b << 8) | a);
}

int	get_r(t_col32 color)
{
	return ((color >> 24) & 0xFF);
}

int	get_g(t_col32 color)
{
	return ((color >> 16) & 0xFF);
}

int	get_b(t_col32 color)
{
	return ((color >> 8) & 0xFF);
}

int	get_a(t_col32 color)
{
	return (color & 0xFF);
}
