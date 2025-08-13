/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   scene_cleanup.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/13 09:51:00 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/13 09:57:38 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "parser.h"
#include "scene.h"

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

	if (ptr == NULL)
		return ;
	mesh = (t_mesh *)ptr;
	free(mesh->obj_path);
	free(mesh->bvh);
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
