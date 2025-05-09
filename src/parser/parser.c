/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/09 10:26:01 by bewong        #+#    #+#                 */
/*   Updated: 2025/05/09 18:22:53 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static bool	parse_scene_ele(char **tokens, t_scene *scene)
{
	if (!tokens || !tokens[0])
		return (false);

	if (ft_strcmp(tokens[0], "A") == 0)
		return (parse_ambient(tokens, &scene->lights));
	else if (ft_strcmp(tokens[0], "C") == 0)
		return (parse_camera(tokens, &scene->camera));
	// else if (ft_strcmp(tokens[0], "L") == 0)
	// 	return (parse_light(tokens, &scene->lights));
	// else if (ft_strcmp(tokens[0], "sp") == 0)
	// 	return (parse_sphere(tokens, &scene->objects));
	// else if (ft_strcmp(tokens[0], "pl") == 0)
	// 	return (parse_plane(tokens, &scene->objects));
	// else if (ft_strcmp(tokens[0], "cy") == 0)
	// 	return (parse_cylinder(tokens, &scene->objects));
	*error() = ERR_UNKNOWN_TOKEN;
	return (false);
}
//add if the file is not exist, print the file name in error message,

bool	parse_map(t_scene *scene, const char *file)
{
	int		fd;
	char	*line;
	char	**tokens;

	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		*error() = ERR_FILE_NONEXIST;
		return(false);
	}
	vector_init(&scene->objects, 5);
	vector_init(&scene->lights, 5);
	while (true)
	{
		line = get_next_line(fd);
		if (line == NULL)
			break ;
		if (line[0] != '#' && line[0] != '\n')
		{
			tokens = ft_split(line, ' ');
			if (tokens == NULL)
				return (free(line), false);
			if (!parse_scene_ele(tokens, scene))
				return (free_tokens(tokens),free(line), false);
			free_tokens(tokens);
		}
		free(line);
	}
	close(fd);
	return (true);
}


