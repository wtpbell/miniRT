/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ray.h                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/16 18:30:15 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/16 18:48:22 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAY_H
# define RAY_H

# include "vector.h"

typedef struct s_ray
{
	t_v3f	origin;
	t_v3f	direction;
}	t_ray;

typedef struct s_ray_hit
{
	t_v3f	hit;
	t_v3f	normal;
	float	distance;
}	t_ray_hit;

#endif
