/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   obj_utils.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/07 19:19:47 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/07 19:36:40 by jboon         ########   odam.nl         */
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

void	construct_mesh_aabb(t_mesh *mesh)
{
	int		i;
	t_tri	*tri;
	t_v3f	min;
	t_v3f	max;

	i = 0;
	min = g_v3f_zero;
	max = g_v3f_zero;
	while (i < mesh->triangles.size)
	{
		tri = (t_tri *)mesh->triangles.items[i];
		min.x = fminf(fminf(fminf(min.x, tri->v0.x), tri->v1.x), tri->v2.x);
		min.y = fminf(fminf(fminf(min.y, tri->v0.y), tri->v1.y), tri->v2.y);
		min.z = fminf(fminf(fminf(min.z, tri->v0.z), tri->v1.z), tri->v2.z);
		max.x = fmaxf(fmaxf(fmaxf(max.x, tri->v0.x), tri->v1.x), tri->v2.x);
		max.y = fmaxf(fmaxf(fmaxf(max.y, tri->v0.y), tri->v1.y), tri->v2.y);
		max.z = fmaxf(fmaxf(fmaxf(max.z, tri->v0.z), tri->v1.z), tri->v2.z);
		++i;
	}
	mesh->box.min = min;
	mesh->box.max = max;
}
