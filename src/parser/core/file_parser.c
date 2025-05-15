/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 16:34:01 by bewong            #+#    #+#             */
/*   Updated: 2025/05/14 19:33:25 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static bool	parse_file_lines(t_scene *scene, int fd)
{
	char	*line;

	while (true)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (*line != '#' && *line != '\n' && !parse_line(scene, line))
			return (cleanup_gnl(line, fd), false);
		free(line);
	}
	return (cleanup_gnl(line, fd), true);
}

bool	parse_map(t_scene *scene, const char *file)
{
	int		fd;
	bool	result;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		perror("Open file failed");
		cleanup_scene(scene);
		return (false);
	}
	result = parse_file_lines(scene, fd);
	close(fd);
	// if (!result)
	// 	cleanup_scene(scene);
	return (result);
}
