/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minirt.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 11:37:50 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/16 14:36:56 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "MLX42/MLX42.h"
# include "scene.h"
# include "color.h"
# include "vector.h"

typedef struct s_ray
{
	t_v3f	origin;
	t_v3f	direction;
}	t_ray;

typedef struct s_ray_hit
{
	t_v3f		hit;
	t_v3f		normal;
	float		distance;
}	t_ray_hit;

int		game(t_scene *scene);
void	render(t_scene *scene);

#endif
