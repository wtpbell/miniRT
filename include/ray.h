/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ray.h                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/02 11:37:29 by bewong        #+#    #+#                 */
/*   Updated: 2025/06/02 11:44:51 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAY_H
# define RAY_H

# include "vector.h"

typedef struct s_object t_obj;


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
	bool	front_face;
	t_obj	*obj;
	t_ray	*ray;
}	t_ray_hit;

#endif


