/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minirt.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 11:37:50 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/20 14:46:59 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "MLX42/MLX42.h"
# include "scene.h"
# include "ray.h"
# include "color.h"
# include "vector.h"

int		game(t_scene *scene);
void	render(t_scene *scene);
bool	solve_quadratic(t_v3f *abc, float *x0, float *x1);
void	obj_to_world(t_mat4x4 dst, t_v3f pos, t_v3f dir, t_v3f up);
int		sphere_intersect(t_obj *obj, t_ray *ray, float *dst);
int		plane_intersect(t_obj *obj, t_ray *ray, float *dst);
int		cylinder_intersect(t_obj *obj, t_ray *ray, float *dst);
t_v3f	sphere_normal(t_obj *obj, t_v3f point);
t_v3f	plane_normal(t_obj *obj, t_v3f point);
t_v3f	cylinder_normal(t_obj *obj, t_v3f point);


#endif
