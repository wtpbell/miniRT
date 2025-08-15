/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_debug.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 10:18:24 by jboon             #+#    #+#             */
/*   Updated: 2025/08/14 20:22:07 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_DEBUG_H
# define RT_DEBUG_H

# include <stdio.h>
# include "scene.h"
# include "rt_math.h"
# include "color.h"
# include "MLX42/MLX42.h"

void	mat4x4_rot_print(t_mat4x4 m);
void	mat4x4_print(t_mat4x4 m, int spaces, const char *prefix);
void	col32_print(t_v3f c, int spaces, const char *prefix);
void	v3f_print(t_v3f v, int spaces, const char *prefix);
void	float_print(float f, int spaces, const char *prefix);
void	cone_print(t_obj *co, int spaces);
void	scene_print(t_scene *scene);
void	print_camera_setup(t_cam *cam);
void	debug_scene_setup(t_scene *scene);

// Bump map debug functions
void	debug_bump_uv(const char *stage, t_v2f uv, t_v3f uvt);
void	debug_bump_sample(mlx_texture_t *bump_map, t_v2f uv, int sample_idx);
void	debug_bump_normal(t_v3f old_normal, t_v3f new_normal);
void	debug_bump_texture_info(mlx_texture_t *bump_map, float delta);

#endif
