/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_debug.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/15 10:18:24 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/15 14:10:44 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_DEBUG_H
# define RT_DEBUG_H

# include <stdio.h>
# include "rt_math.h"
# include "color.h"

void	mat4x4_rot_print(t_mat4x4 m);
void	mat4x4_print(t_mat4x4 m);
void	col32_print(t_col32 c);
void	v3f_print(t_v3f v);

#endif
