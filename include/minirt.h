/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minirt.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 11:37:50 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/15 13:32:47 by bewong        ########   odam.nl         */
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

// Core Functions
void		quit_on_escape(mlx_key_data_t keydata, void *param);
void		init_object_matrices(t_obj *obj);
void		init_object_transform(t_obj *obj, t_v3f pos, t_v3f dir, t_v3f up);
void		init_object_renderer(t_obj *obj, t_v3f col, t_texcoord coord);
void		*render(void *ctx);
bool		solve_quadratic(t_v3f *abc, float *x0, float *x1);
int			sphere_intersect(t_obj *obj, t_ray *ray, t_v2f t, t_result *res);
int			plane_intersect(t_obj *obj, t_ray *ray, t_v2f t, t_result *res);
int			cylinder_intersect(t_obj *obj, t_ray *ray, t_v2f t, t_result *res);
int			triangle_intersect(t_obj *obj, t_ray *ray, t_v2f t, t_result *res);
int			mesh_intersect(t_obj *obj, t_ray *ray, t_v2f t, t_result *res);
int			cone_intersect(t_obj *obj, t_ray *ray, t_v2f t, t_result *res);
int			tri_intersect(t_tri *tri, t_ray *ray, t_v2f t, t_result *res);
int			intersect_disc(float r, float h, t_ray *ray, t_v2f *t_lim);
bool		aabb_intersect(t_ray *ray, t_aabb *box);
t_obj		*find_intersection(t_ray *ray, t_scene *scene, t_result *res);
t_v3f		trace(t_ray *ray, t_scene *scene, uint32_t depth);
t_v3f		sphere_normal(t_obj *obj, t_v3f point, t_result *res);
t_v3f		plane_normal(t_obj *obj, t_v3f point, t_result *res);
t_v3f		cylinder_normal(t_obj *obj, t_v3f point, t_result *res);
t_v3f		triangle_normal(t_obj *obj, t_v3f point, t_result *res);
t_v3f		cone_normal(t_obj *obj, t_v3f point, t_result *res);
t_v3f		mesh_normal(t_obj *obj, t_v3f point, t_result *res);
t_ray		get_ray_with_dof(t_cam *cam, float u, float v);
void		update_camera_view(t_cam *cam);
void		generate_uv_vertices(t_tri *tri, t_mat4x4 local);
bool		valid_file_format(const char *file, const char *ext);
t_bhv_node	*construct_bvh(t_mesh *mesh);
void		calc_tri_centroid(t_tri *triangles, int count);
void		tri_swap(t_tri *a, t_tri *b);

/* scene_cleanup.c */
void		free_material(void *ptr);
void		free_obj(void *ptr);
void		free_mesh(void *ptr);
void		cleanup_scene(t_scene *scene);

#endif
