/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_debug.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/15 10:18:24 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/16 15:36:42 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_DEBUG_H
# define RT_DEBUG_H

# include <stdio.h>
# include "scene.h"
# include "rt_math.h"
# include "color.h"

void	mat4x4_rot_print(t_mat4x4 m);
void	mat4x4_print(t_mat4x4 m, int spaces, const char *prefix);
void	col32_print(t_col32 c, int spaces, const char *prefix);
void	v3f_print(t_v3f v, int spaces, const char *prefix);
void	scene_print(t_scene *scene);

#endif
