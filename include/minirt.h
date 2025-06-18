/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minirt.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 11:37:50 by jboon         #+#    #+#                 */
/*   Updated: 2025/06/18 16:28:05 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "MLX42/MLX42.h"
# include "scene.h"
# include "color.h"
# include "vector.h"

typedef struct s_game
{
	mlx_t	*mlx;
	/* TODO: Add more members here for access during a MLX hook */
}	t_game;

int		game(t_scene *scene);
void	quit_on_escape(mlx_key_data_t keydata, void *param);
void	init_object_matrices(t_obj *obj);
void	init_obj_transform(t_obj *obj, t_v3f pos, t_v3f dir, t_v3f up);
void	init_obj_renderer(t_obj *obj, t_v3f col, t_texcoord coord);
void	render(t_scene *scene);
bool	solve_quadratic(t_v3f *abc, float *x0, float *x1);
void	obj_to_world(t_mat4x4 dst, t_v3f pos, t_v3f dir, t_v3f up);
int		sphere_intersect(t_obj *obj, t_ray *ray, t_v2f t, float *dst);
int		plane_intersect(t_obj *obj, t_ray *ray, t_v2f t, float *dst);
int		cylinder_intersect(t_obj *obj, t_ray *ray, t_v2f t, float *dst);
int		triangle_intersect(t_obj *obj, t_ray *ray, t_v2f t, float *dst);
t_obj	*find_intersection(t_ray *ray, t_scene *scene, float *t);
t_v3f	trace(t_ray *ray, t_scene *scene, uint32_t depth);
t_v3f	sphere_normal(t_obj *obj, t_v3f point);
t_v3f	plane_normal(t_obj *obj, t_v3f point);
t_v3f	cylinder_normal(t_obj *obj, t_v3f point);
t_v3f	triangle_normal(t_obj *obj, t_v3f point);
void	debug_scene_setup(t_scene *scene);
void	generate_uv_vertices(t_tri *tri, t_mat4x4 local);

#endif
