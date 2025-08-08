/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   construct_mesh.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/07 18:35:30 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/08 17:15:45 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "errno.h"

static bool	parse_line(char *line, t_obj_file *obj_file)
{
	const t_vert_ran	ran[3] = {
		(t_vert_ran){init_v2f(1, 4), init_v2f(MIN_POS, MAX_POS),
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

static bool	parse_obj_file(t_mesh *mesh, t_mat4x4 local)
{
	const char	*file_name = mesh->obj_path;
	char		*line;
	int			fd;
	t_obj_file	obj_file;

	fd = open(file_name, O_RDONLY);
	if (fd < 0 || !init_obj_file(&obj_file))
		return (perror("parse mesh"), close(fd), false);
	errno = 0;
	while (true)
	{
		line = get_next_line(fd);
		if (line == NULL)
			break ;
		if (!parse_line(line, &obj_file))
			return (cleanup_gnl(line, fd), free_obj_file(&obj_file), false);
		free(line);
	}
	close (fd);
	get_next_line(-1);
	if (errno != 0)
		return (perror("parse mesh"), free_obj_file(&obj_file), false);
	if (!load_obj_into_mesh(&obj_file, mesh, local))
		return (free_obj_file(&obj_file), false);
	return (free_obj_file(&obj_file), true);
}

bool	construct_mesh(t_scene *scene)
{
	t_obj	*obj;
	int		i;
	int		size;

	i = 0;
	size = scene->objects.size;
	while (i < size)
	{
		obj = (t_obj *)scene->objects.items[i];
		// TODO: Cache the obj files for faster re-use
		if (obj->type == OBJ_MESH && !parse_obj_file(&obj->mesh, obj->t.to_obj))
			return (print_error(ERR_OBJ_FAIL, "obj", obj->mesh.obj_path),
				false);
		++i;
	}
	return (true);
}
