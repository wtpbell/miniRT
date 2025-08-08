/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   obj.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/02 15:58:08 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/08 11:26:25 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "errno.h"

bool	init_obj_file(t_obj_file *obj_file)
{
	ft_bzero(obj_file, sizeof(obj_file));
	if (!vector_init(&obj_file->v, 3)
		|| !vector_init(&obj_file->vt, 3)
		|| !vector_init(&obj_file->vn, 3)
		|| !vector_init(&obj_file->f, 3))
	{
		vector_free(&obj_file->v, free);
		vector_free(&obj_file->vt, free);
		vector_free(&obj_file->vn, free);
		return (false);
	}
	return (true);
}

void	free_obj_file(t_obj_file *obj_file)
{
	vector_free(&obj_file->v, free);
	vector_free(&obj_file->vt, free);
	vector_free(&obj_file->vn, free);
	vector_free(&obj_file->f, free);
}

static bool	init_vertices(t_tri *tri, t_obj_file *obj_file, int face_index,
	t_mat4x4 local)
{
	int	*indices;

	indices = obj_file->f.items[face_index];
	if (!assign_v3f((t_v3f *[3]){&tri->v0, &tri->v1, &tri->v2}, &obj_file->v,
		indices, 0))
		return (false);
	if (*(indices + 1) == 0)
		generate_uv_vertices(tri, local);
	else
		if (!assign_v3f((t_v3f *[3]){&tri->vt0, &tri->vt1, &tri->vt2},
			&obj_file->v, indices, 1))
			return (false);
	if (*(indices + 2) == 0)
		set_normal(tri);
	else
	{
		if (!assign_v3f((t_v3f *[3]){&tri->vn0, &tri->vn1, &tri->vn2},
			&obj_file->v, indices, 1))
			return (false);
		tri->vn0 = v3f_norm(tri->vn0);
		tri->vn1 = v3f_norm(tri->vn1);
		tri->vn2 = v3f_norm(tri->vn2);
	}
	return (true);
}

bool	load_obj_into_mesh(t_obj_file *obj_file, t_mesh *mesh, t_mat4x4 local)
{
	int		i;
	t_tri	*tri;

	i = 0;
	while (i < obj_file->f.size)
	{
		tri = ft_calloc(1, sizeof(t_tri));
		if (tri == NULL)
			return (false);
		if (!init_vertices(tri, obj_file, i, local))
			return (free(tri), false);
		if (!vector_add(&mesh->triangles, tri))
			return (free(tri), false);
		++i;
	}
	construct_mesh_aabb(mesh);
	return (true);
}
