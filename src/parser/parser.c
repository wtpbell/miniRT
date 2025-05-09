/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/09 10:26:01 by bewong        #+#    #+#                 */
/*   Updated: 2025/05/09 15:02:04 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

bool	parse_scene_ele(char **tokens, t_scene *scene)
{
	if (!tokens || !tokens[0])
		return (false);

	if (ft_strcmp(tokens[0], "A") == 0)
		return (parse_ambient(tokens, &scene->ambient));
	else if (ft_strcmp(tokens[0], "C") == 0)
		return (parse_camera(tokens, &scene->camera));
	else if (ft_strcmp(tokens[0], "L") == 0)
		return (parse_light(tokens, &scene->lights));
	else if (ft_strcmp(tokens[0], "sp") == 0)
		return (parse_sphere(tokens, &scene->objects));
	else if (ft_strcmp(tokens[0], "pl") == 0)
		return (parse_plane(tokens, &scene->objects));
	else if (ft_strcmp(tokens[0], "cy") == 0)
		return (parse_cylinder(tokens, &scene->objects));
	*error() = ERR_UNKNOWN_TOKEN;
	return (false);
}


bool parse_map(t_scene *scene, const char *file)
{
	int		fd;
	char	*line;
	char	**tokens;

	fd = open(file, O_RDONLY | O_WRONLY);
	if (fd == -1)
		return (false);
	vector_init(&scene->objects, 5);
	vector_init(&scene->lights, 5);
	while (true)
	{
		line = get_next_line(fd);
		if (line == NULL)
			break;
		if (line[0] != '#' && line[0] != '\n')
		{
			tokens = ft_split(line, ' ');
			if (tokens == NULL)
				return (free(line), false);
			if (!parse_scene_ele(tokens, scene))
				return (ft_split_free(tokens),free(line), false);
			ft_split_free(tokens);
		}
		free(line);
	}
	close(fd);
	return (true);
}


