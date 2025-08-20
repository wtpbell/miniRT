/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:16:29 by jboon             #+#    #+#             */
/*   Updated: 2025/08/20 18:03:18 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLOR_H
# define COLOR_H

# include "MLX42/MLX42.h"
# include "vector.h"
# include <stdint.h>

# define C_RED		0xFF0000FF
# define C_GREEN	0x00FF00FF
# define C_BLUE		0x0000FFFF
# define C_WHITE	0xFFFFFFFF
# define C_BLACK	0x000000FF
# define C_TRANS	UI_TRANSPARENT

typedef uint32_t	t_col32;

t_col32		init_col32(uint32_t r, uint32_t g, uint32_t b, uint32_t a);
int			get_r(t_col32 color);
int			get_g(t_col32 color);
int			get_b(t_col32 color);
int			get_a(t_col32 color);
t_v3f		col32_to_v3f(t_col32 color);
t_col32		v3f_to_col32(t_v3f color);
t_col32		col32_lerp(t_col32 a, t_col32 b, float t);
t_v3f		v3f_apply_gamma(t_v3f color, float gamma);
uint32_t	rt_get_pixel(mlx_image_t *img, uint32_t x, uint32_t y);

#endif
