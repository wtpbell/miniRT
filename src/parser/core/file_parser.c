/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   file_parser.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 16:34:01 by bewong        #+#    #+#                 */
/*   Updated: 2025/05/12 18:10:39 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static bool	parse_file_lines(t_scene *scene, int fd)
{
	char	*line;
	bool	result;

	result = true;
	while (true)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (line[0] == '#' || line[0] == '\n')
		{
			free(line);
			continue ;
		}
		if (!parse_line(scene, line))
		{
			free(line);
			result = false;
			break ;
		}
		free(line);
	}
	cleanup_gnl(NULL, fd);
	return (result);
}

bool	parse_map(t_scene *scene, const char *file)
{
	int		fd;
	bool	result;

	if (!vector_init(&scene->objects, 8)
		|| !vector_init(&scene->lights, 8))
	{
		*error() = ERR_MEM;
		return (false);
	}
	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		*error() = ERR_FILE_NONEXIST;
		vector_free(&scene->objects, del_objects);
		vector_free(&scene->lights, del_lights);
		return (false);
	}
	result = parse_file_lines(scene, fd);
	close(fd);
	if (!result)
	{
		vector_free(&scene->objects, del_objects);
		vector_free(&scene->lights, del_lights);
	}
	return (result);
}
