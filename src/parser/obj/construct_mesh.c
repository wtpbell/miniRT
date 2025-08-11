/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   construct_mesh.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/07 18:35:30 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/11 22:28:20 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "errno.h"
#include "libft.h"

static bool	parse_line(char *line, t_obj_file *obj_file)
{
	const t_vert_ran	ran[3] = {
		(t_vert_ran){init_v2f(3, 3), init_v2f(MIN_POS, MAX_POS),
		init_v4f(0.0f, 0.0f, 0.0f, 1.0f)},
		(t_vert_ran){init_v2f(1, 3), init_v2f(0.0f, 1.0f),
		init_v4f(1.0f, 1.0f, 1.0f, 1.0f)},
		(t_vert_ran){init_v2f(3, 3), init_v2f(MIN_POS, MAX_POS),
		init_v4f(0.0f, 0.0f, 0.0f, 0.0f)}};
	char				*token;
	char				*saveptr;

	token = rt_strtok(line, WHITESPACE, &saveptr);
	if (token == NULL || *token == '#')
		return (true);
	if (ft_strcmp(token, "f") == 0)
		return (parse_face(saveptr, &obj_file->f));
	else if (ft_strcmp(token, "v") == 0)
		return (parse_vertex(saveptr, &obj_file->v, &ran[0]));
	else if (ft_strcmp(token, "vt") == 0)
		return (parse_vertex(saveptr, &obj_file->vt, &ran[1]));
	else if (ft_strcmp(token, "vn") == 0)
		return (parse_vertex(saveptr, &obj_file->vn, &ran[2]));
	return (true);
}

static t_mesh	*parse_obj_file(const char *obj_path)
{
	char		*line;
	int			fd;
	t_obj_file	obj_file;
	t_mesh		*mesh;

	fd = open(obj_path, O_RDONLY);
	if (fd < 0 || !init_obj_file(&obj_file))
		return (perror("parse mesh"), cleanup_gnl(NULL, fd), NULL);
	errno = 0;
	while (true)
	{
		line = get_next_line(fd);
		if (line == NULL)
			break ;
		if (!parse_line(line, &obj_file))
			return (cleanup_gnl(line, fd), free_obj_file(&obj_file), NULL);
		free(line);
	}
	cleanup_gnl(NULL, fd);
	if (errno != 0)
		return (perror("parse mesh"), free_obj_file(&obj_file), NULL);
	mesh = load_obj_into_mesh(obj_path, &obj_file);
	return (free_obj_file(&obj_file), mesh);
}

static int	find_shared_mesh(int i, void *item, void *ctx)
{
	(void)i;
	return (ft_strcmp(((t_mesh *)item)->obj_path, (const char *)ctx));
}

bool	construct_mesh(t_scene *scene)
{
	t_obj	*obj;
	t_mesh	*shared_mesh;
	int		i;
	int		size;

	i = 0;
	size = scene->objects.size;
	while (i < size)
	{
		obj = (t_obj *)scene->objects.items[i];
		if (obj->type == OBJ_MESH)
		{
			shared_mesh = (t_mesh *)vector_find(&scene->shared_mesh, obj->mesh.obj_path, find_shared_mesh);
			if (shared_mesh == NULL)
			{
				shared_mesh = parse_obj_file(obj->mesh.obj_path); // TODO: What if it is an empty file? What about no vertices or faces?
				if (shared_mesh == NULL || !vector_add(&scene->shared_mesh, shared_mesh))
					return (free_mesh(shared_mesh), perror("obj"), false);
			}
			// I would have prefered to just pass store the pointer of shared_mesh directly, but the obj_path is preventing that.
			obj->mesh.bvh = shared_mesh->bvh;
			obj->mesh.tri_count = shared_mesh->tri_count;
			obj->mesh.triangles = shared_mesh->triangles;
		}
		++i;
	}
	return (true);
}
