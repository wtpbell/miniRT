/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cleanup.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 16:44:01 by bewong        #+#    #+#                 */
/*   Updated: 2025/08/10 14:06:29 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "material.h"
#include "debug/rt_debug.h"

void	free_tokens(char **tokens)
{
	size_t	i;

	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

void	cleanup_gnl(char *line, int fd)
{
	free(line);
	close(fd);
	get_next_line(-1);
}

void	free_material(void *ptr)
{
	t_mat	*mat;

	mat = (t_mat *)ptr;
	free(mat->name);
	free(mat->tex_path);
	free(mat->bump_path);
	if (mat->bump_map)
		mlx_delete_texture(mat->bump_map);
	cleanup_texture(&mat->texture);
	free(ptr);
}

void	free_obj(void *ptr)
{
	t_obj	*obj;

	obj = (t_obj *)ptr;
	if (obj != NULL && obj->type == OBJ_MESH)
		free(obj->mesh.obj_path);
	free(obj);
}

void	free_mesh(void *ptr)
{
	t_mesh	*mesh;

	mesh = (t_mesh *)ptr;
	free(mesh->obj_path);
	free(mesh->triangles);
	free(ptr);
}

void	cleanup_scene(t_scene *scene)
{
	vector_free(&scene->objects, free_obj);
	vector_free(&scene->lights, free);
	vector_free(&scene->shared_materials, free_material);
	vector_free(&scene->shared_mesh, free_mesh);
}
