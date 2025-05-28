/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_utils.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 16:05:00 by cascade          #+#    #+#             */
/*   Updated: 2025/05/28 16:05:00 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLOR_UTILS_H
# define COLOR_UTILS_H

# include "color.h"
# include "vector.h"

t_v3f	col32_to_v3f(t_col32 color);
t_col32	v3f_to_col32(t_v3f color);
t_col32	lerp_col32(t_col32 a, t_col32 b, float t);
t_col32	add_colors_clamped(t_col32 a, t_col32 b);
t_col32	apply_gamma(t_col32 color, float gamma);

#endif
