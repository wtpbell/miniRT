/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 11:37:50 by jboon             #+#    #+#             */
/*   Updated: 2025/05/18 21:26:00 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "MLX42/MLX42.h"
# include "scene.h"
# include "ray.h"
# include "color.h"
# include "vector.h"

# define EPSILON 1e-3f

int		game(t_scene *scene);
void	render(t_scene *scene);
bool	solve_quadratic(t_v3f *abc, float *x0, float *x1);
void	obj_to_world(t_mat4x4 dst, t_v3f pos, t_v3f dir);
int		sphere_intersect(t_obj *obj, t_ray *ray, float *dst);
int		cylinder_intersect(t_obj *obj, t_ray *ray, float *dst);

#endif
