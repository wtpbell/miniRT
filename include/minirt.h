/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 11:37:50 by jboon             #+#    #+#             */
/*   Updated: 2025/08/07 21:21:35 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <stdbool.h>
# include <stdint.h>
# include <stddef.h>
# include "MLX42/MLX42.h"
# include "scene.h"
# include "color.h"
# include "vector.h"
# include "game.h"

// Core Functions
void	quit_on_escape(mlx_key_data_t keydata, void *param);
void	init_object_matrices(t_obj *obj);
void	init_obj_transform(t_obj *obj, t_v3f pos, t_v3f dir, t_v3f up);
void	init_obj_renderer(t_obj *obj, t_v3f col, t_texcoord coord);
void	*render(void *ctx);
bool	solve_quadratic(t_v3f *abc, float *x0, float *x1);
int		sphere_intersect(t_obj *obj, t_ray *ray, t_v2f t, t_v3f *scalar);
int		plane_intersect(t_obj *obj, t_ray *ray, t_v2f t, t_v3f *scalar);
int		cylinder_intersect(t_obj *obj, t_ray *ray, t_v2f t, t_v3f *scalar);
int		triangle_intersect(t_obj *obj, t_ray *ray, t_v2f t, t_v3f *scalar);
int		cone_intersect(t_obj *obj, t_ray *ray, t_v2f t, t_v3f *scalar);
int		intersect_disc(float r, float h, t_ray *ray, t_v2f *t_lim);
t_obj	*find_intersection(t_ray *ray, t_scene *scene, t_v3f *t);
t_v3f	trace(t_ray *ray, t_scene *scene, uint32_t depth);
t_v3f	sphere_normal(t_obj *obj, t_v3f point);
t_v3f	plane_normal(t_obj *obj, t_v3f point);
t_v3f	cylinder_normal(t_obj *obj, t_v3f point);
t_v3f	triangle_normal(t_obj *obj, t_v3f point);
t_v3f	cone_normal(t_obj *obj, t_v3f point);
t_ray	get_ray_with_dof(t_cam *cam, float u, float v);
void	update_camera_view(t_cam *cam);
void	generate_uv_vertices(t_tri *tri, t_mat4x4 local);
#endif
