/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_mesh.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/02 17:28:29 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/02 17:32:11 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "scene.h"
#include "rt_math.h"
#include "ray.h"


// TODO: Implementation needed
t_v3f	mesh_normal(t_obj *obj, t_v3f point)
{
	(void)obj;
	(void)point;
	return (g_v3f_up);
}

// TODO: Implementation needed
t_v2f	mesh_texcoord(t_obj *obj, t_v3f world_point)
{
	(void)obj;
	(void)world_point;
	return (g_v2f_zero);
}

// TODO: Implementation needed
bool	mesh_intersect(t_obj *obj, t_ray *ray, t_v2f t, float *dst)
{
	(void)obj;
	(void)ray;
	(void)t;
	(void)dst;
	return (false);
}
