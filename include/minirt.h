/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 11:37:50 by jboon             #+#    #+#             */
/*   Updated: 2025/07/23 22:37:22 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <stdbool.h>
# include <stdint.h>
# include <stddef.h>
# include "MLX42/MLX42.h"
# include "ui.h"
# include "scene.h"
# include "color.h"
# include "vector.h"

// Rendering
void	render_scene(t_game *game);

// UI Functions
// void			rt_put_string(mlx_image_t *img, const char *str, int x, int y);
// void			ui_init(t_ui *ui, t_scene *scene);
// void			draw_ui(t_game *game);
// void			init_ambient_buttons(t_ui *ui, t_scene *scene);
// void			init_camera_buttons(t_ui *ui, t_cam *camera);
// void			init_point_light_buttons(t_ui *ui, t_scene *scene);
// bool			is_point_in_button(t_button *button, int x, int y, int y_offset);
// void			update_button_value(t_button *button, int mouse_x, int y_offset);
// void			draw_button(mlx_image_t *img, t_button *button, int y_offset);
// void			draw_section_header(mlx_image_t *img, const char *title, int x, int y);
// void			draw_submit_button(mlx_image_t *img, const char *title, int x, int y);
// // Game functions
// bool			game_init(t_game *game, t_scene *scene);
// int				game(t_scene *scene);
// void			update_wrapper(void *param);
// void			cleanup_mlx(t_game *game);
// bool			cam_init(t_cam *camera, mlx_t *mlx);	
// // Input handlers
// void			key_hook(mlx_key_data_t keydata, void *param);
// void			cursor_hook(double xpos, double ypos, void *param);
// void			mouse_hook(mouse_key_t button, action_t action, modifier_key_t mods, void *param);

// Core Functions
void	quit_on_escape(mlx_key_data_t keydata, void *param);
void	init_object_matrices(t_obj *obj);
void	init_obj_transform(t_obj *obj, t_v3f pos, t_v3f dir, t_v3f up);
void	init_obj_renderer(t_obj *obj, t_v3f col, t_texcoord coord);
void	render(t_scene *scene);
bool	solve_quadratic(t_v3f *abc, float *x0, float *x1);
int		sphere_intersect(t_obj *obj, t_ray *ray, t_v2f t, float *dst);
int		plane_intersect(t_obj *obj, t_ray *ray, t_v2f t, float *dst);
int		cylinder_intersect(t_obj *obj, t_ray *ray, t_v2f t, float *dst);
int		triangle_intersect(t_obj *obj, t_ray *ray, t_v2f t, float *dst);
int		cone_intersect(t_obj *obj, t_ray *ray, t_v2f t, float *dst);
int		intersect_disc(float r, float h, t_ray *ray, t_v2f *t_lim);
t_obj	*find_intersection(t_ray *ray, t_scene *scene, float *t);
t_v3f	trace(t_ray *ray, t_scene *scene, uint32_t depth);
t_v3f	sphere_normal(t_obj *obj, t_v3f point);
t_v3f	plane_normal(t_obj *obj, t_v3f point);
t_v3f	cylinder_normal(t_obj *obj, t_v3f point);
t_v3f	triangle_normal(t_obj *obj, t_v3f point);
t_v3f	cone_normal(t_obj *obj, t_v3f point);
void	debug_scene_setup(t_scene *scene);
t_ray	get_ray_with_dof(t_cam *cam, float u, float v);
void	update_camera_view(t_cam *cam);
void	generate_uv_vertices(t_tri *tri, t_mat4x4 local);
void	render_scene(t_game *game);
// In minirt.h
int game(t_scene *scene);
#endif
