/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:16:29 by jboon             #+#    #+#             */
/*   Updated: 2025/06/13 19:09:39 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLOR_H
# define COLOR_H

# include "vector.h"
# include <stdint.h>
typedef uint32_t	t_col32;

t_col32	init_col32(uint32_t r, uint32_t g, uint32_t b, uint32_t a);
int		get_r(t_col32 color);
int		get_g(t_col32 color);
int		get_b(t_col32 color);
int		get_a(t_col32 color);
t_v3f	col32_to_v3f(t_col32 color);
t_col32	v3f_to_col32(t_v3f color);
t_col32	col32_lerp(t_col32 a, t_col32 b, float t);
t_v3f	v3f_apply_gamma(t_v3f color, float gamma);
#endif
