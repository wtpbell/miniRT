/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minirt.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 11:37:50 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/20 11:05:15 by jboon         ########   odam.nl         */
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
int		sphere_intersect(t_obj *obj, t_ray *ray, float *dst);
t_v3f	sphere_normal(t_obj *obj, t_v3f point);
int		plane_intersect(t_obj *obj, t_ray *ray, float *dst);
t_v3f	plane_normal(t_obj *obj, t_v3f point);

#endif
