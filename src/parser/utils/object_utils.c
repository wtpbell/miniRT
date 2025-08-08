/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   obj_utils.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/10 10:17:07 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/08 10:39:14 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "scene.h"
#include "vector.h"

void	init_object_matrices(t_obj *obj)
{
	if (obj->t.up.x != 0 || obj->t.up.y != 0 || obj->t.up.z != 0)
		obj_to_world(obj->t.to_world, obj->t.pos, obj->t.dir, obj->t.up);
	else
		obj_to_world(obj->t.to_world, obj->t.pos, obj->t.dir, g_v3f_up);
	invert_m4x4(obj->t.to_obj, obj->t.to_world);
}

void	init_object_transform(t_obj *obj, t_v3f pos, t_v3f dir, t_v3f up)
{
	obj->t.pos = pos;
	obj->t.dir = dir;
	obj->t.up = up;
	init_object_matrices(obj);
}

void	init_object_renderer(t_obj *obj, t_v3f col, t_texcoord coord)
{
	obj->r.color = col;
	obj->r.get_texcoord = coord;
}
