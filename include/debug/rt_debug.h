/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_debug.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 10:18:24 by jboon             #+#    #+#             */
/*   Updated: 2025/06/25 22:35:32 by bewong           ###   ########.fr       */
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
void	col32_print(t_v3f c, int spaces, const char *prefix);
void	v3f_print(t_v3f v, int spaces, const char *prefix);
void	float_print(float f, int spaces, const char *prefix);
void	cone_print(t_obj *co, int spaces);
void	scene_print(t_scene *scene);
void	print_camera_setup(t_cam *cam);

#endif
