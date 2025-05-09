/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   color.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/08 19:16:29 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/09 16:19:53 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLOR_H
# define COLOR_H

# include <stdint.h>

# define C_RED		0xFF0000FF
# define C_GREEN	0x00FF00FF
# define C_BLUE		0x0000FFFF
# define C_WHITE	0xFFFFFFFF
# define C_BLACK	0x000000FF

typedef uint32_t	t_col32;

t_col32	init_col32(uint32_t r, uint32_t g, uint32_t b, uint32_t a);
int		get_r(t_col32 color);
int		get_g(t_col32 color);
int		get_b(t_col32 color);
int		get_a(t_col32 color);

#endif
