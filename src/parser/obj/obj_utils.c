/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   obj_utils.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/07 19:19:47 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/15 14:36:26 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

bool	assign_v3f(t_v3f *dst[3], t_vector *cont, int *indices, int curr)
{
	int	i;

	while (curr < MAX_VERT_PER_SET)
	{
		i = indices[curr] - 1;
		if (vector_get(cont, i) == NULL)
			return (false);
		*dst[curr / 3] = *((t_v3f *)cont->items[i]);
		curr += 3;
	}
	return (true);
}

void	set_normal(t_tri *tri)
{
	t_v3f	norm;

	norm = v3f_norm(
			v3f_cross(v3f_sub(tri->v1, tri->v0), v3f_sub(tri->v2, tri->v0)));
	tri->vn0 = norm;
	tri->vn1 = norm;
	tri->vn2 = norm;
}

void	set_uv_texcoord(t_tri *tri)
{
	t_mat4x4	world;
	t_mat4x4	local;

	id_m4x4(local);
	id_m4x4(world);
	obj_to_world(world, get_mid_point(tri->v0, tri->v1, tri->v2),
		get_normal(tri->v0, tri->v1, tri->v2), g_v3f_up);
	invert_m4x4(local, world);
	generate_uv_vertices(tri, local);
}
