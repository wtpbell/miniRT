/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   obj.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/02 15:58:08 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/10 20:01:22 by jboon         ########   odam.nl         */
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

static t_mesh	*init_mesh(const char *obj_path, t_obj_file *obj_file)
{
	t_mesh	*mesh;
	
	mesh = malloc(sizeof(t_mesh));
	if (mesh == NULL)
		return (NULL);
	mesh->obj_path = ft_strdup(obj_path);
	mesh->tri_count = obj_file->f.size;
	mesh->triangles = malloc(mesh->tri_count * sizeof(t_tri));
	if (mesh->obj_path == NULL || mesh->triangles == NULL)
		return (free_mesh(mesh), NULL);
	return (mesh);
}

static bool	init_vertices(t_tri *tri, t_obj_file *obj_file, int face_index,
	t_mat4x4 local)
{
	int	*indices;

	indices = obj_file->f.items[face_index];
	if (!assign_v3f((t_v3f *[3]){&tri->v0, &tri->v1, &tri->v2}, &obj_file->v,
		indices, 0))
		return (print_error(ERR_OBJ_VERT_INDEX, "obj - v", NULL), false);
	if (*(indices + 1) == 0)
		generate_uv_vertices(tri, local);
	else
		if (!assign_v3f((t_v3f *[3]){&tri->vt0, &tri->vt1, &tri->vt2},
			&obj_file->vt, indices, 1))
			return (print_error(ERR_OBJ_VERT_INDEX, "obj - vt", NULL), false);
	if (*(indices + 2) == 0)
		set_normal(tri);
	else
	{
		if (!assign_v3f((t_v3f *[3]){&tri->vn0, &tri->vn1, &tri->vn2},
			&obj_file->vn, indices, 2))
			return (print_error(ERR_OBJ_VERT_INDEX, "obj - vn", NULL), false);
		tri->vn0 = v3f_norm(tri->vn0);
		tri->vn1 = v3f_norm(tri->vn1);
		tri->vn2 = v3f_norm(tri->vn2);
	}
	return (true);
}

t_mesh	*load_obj_into_mesh(const char *obj_path, t_obj_file *obj_file)
{
	int			i;
	t_tri		*tri;
	t_mesh		*mesh;
	t_mat4x4	local;

	if (obj_file->f.size == 0)
		return (NULL); // TODO: PRINT ERROR
	mesh = init_mesh(obj_path, obj_file);
	if (mesh == NULL)
		return (perror("minirt"), NULL);
	i = 0;
	id_m4x4(local);
	while (i < obj_file->f.size)
	{
		tri = (mesh->triangles + i);
		if (!init_vertices(tri, obj_file, i, local))
			return (free_mesh(mesh), NULL);
		++i;
	}
	mesh->bhv = construct_hierarchy_aabb(mesh);
	if (mesh->bhv == NULL)
		return (free_mesh(mesh), perror("minirt"), NULL);
	return (mesh);
}
