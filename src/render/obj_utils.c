/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   obj_utils.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/10 10:17:07 by jboon         #+#    #+#                 */
/*   Updated: 2025/06/10 10:31:21 by jboon         ########   odam.nl         */
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
