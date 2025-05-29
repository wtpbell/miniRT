/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minirt.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 11:37:50 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/29 14:28:09 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "MLX42/MLX42.h"
# include "scene.h"
# include "color.h"
# include "vector.h"

int			game(t_scene *scene);
void		init_object_matrices(t_obj *obj);
void		render(t_scene *scene);
bool		solve_quadratic(t_v3f *abc, float *x0, float *x1);
void		obj_to_world(t_mat4x4 dst, t_v3f pos, t_v3f dir, t_v3f up);
int			sphere_intersect(t_obj *obj, t_ray *ray, t_v2f t, float *dst);
int			plane_intersect(t_obj *obj, t_ray *ray, t_v2f t, float *dst);
int			cylinder_intersect(t_obj *obj, t_ray *ray, t_v2f t, float *dst);
t_v3f		sphere_normal(t_obj *obj, t_v3f point);
t_v3f		plane_normal(t_obj *obj, t_v3f point);
t_v3f		cylinder_normal(t_obj *obj, t_v3f point);
t_col32		trace(t_ray *ray, t_scene *scene, uint32_t depth);
uint32_t	get_rngstate(uint32_t x, uint32_t y, uint32_t frame);

#endif
