/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mesh.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/02 17:01:40 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/08 10:39:14 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static bool	parse_path(char **obj_path, const char *token)
{
	*obj_path = NULL;
	if (valid_file_format(token, ".obj"))
		*obj_path = ft_strdup(token);
	return (*obj_path != NULL);
}

static bool	mesh_init(t_obj *obj, char *obj_path)
{
	obj->type = OBJ_MESH;
	obj->mesh.obj_path = obj_path;
	obj->intersect = mesh_intersect;
	obj->calc_norm = mesh_normal;
	if (!vector_init(&obj->mesh.triangles, 10))
		return (false);
	return (true);
}

bool	parse_mesh(char **tokens, t_scene *scene)
{
	t_v3f	pos;
	t_v3f	dir;
	t_v3f	color;
	char	*obj_path;
	t_obj	*obj;

	if (!parse_v3f(&pos, tokens[1]) || !parse_dir(&dir, tokens[2])
		|| !parse_col(&color, tokens[3]) || !parse_path(&obj_path, tokens[4]))
		return (false);
	obj = ft_calloc(1, sizeof(t_obj));
	if (!obj)
		return (free(obj_path), false);
	init_object_transform(obj, pos, dir, g_v3f_up);
	init_object_renderer(obj, color, mesh_texcoord);
	if (!mesh_init(obj, obj_path)
		|| !assign_material(obj, &scene->shared_materials, tokens[5])
		|| !vector_add(&scene->objects, obj))
		return (free_obj(obj), false);
	return (true);
}
