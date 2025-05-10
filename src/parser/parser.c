/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 10:26:01 by bewong            #+#    #+#             */
/*   Updated: 2025/05/10 18:54:41 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static bool	parse_scene_element(char **tokens, t_scene *scene)
{
	char	*trimmed;
	bool	result;

	if (!tokens || !tokens[0])
		return (false);
	result = false;
	printf("Parsing scene element:\n");
	for (int i = 0; tokens[i] != NULL; i++)
		printf("Token %d: '%s'\n", i, tokens[i]);
	trimmed = ft_strtrim(tokens[0], " \t");
	if (!trimmed)
		return (false);
	// if (ft_strcmp(trimmed, "A") == 0)
	//     result = parse_ambient(tokens, &scene->lights);
	// else if (ft_strcmp(trimmed, "C") == 0)
	//     result = parse_camera(tokens, &scene->camera);
	// else if (ft_strcmp(trimmed, "L") == 0)
	//     result = parse_light(tokens, &scene->lights);
	if (ft_strcmp(trimmed, "sp") == 0)
		result = parse_sphere(tokens, &scene->objects);
	// else if (ft_strcmp(trimmed, "pl") == 0)
	//     result = parse_plane(tokens, &scene->objects);
	// else if (ft_strcmp(trimmed, "cy") == 0)
	//     result = parse_cylinder(tokens, &scene->objects);
	else
	{
		*error() = ERR_UNKNOWN_TOKEN;
		result = false;
	}
	free(trimmed);
	return result;
}

static bool	parse_line(t_scene *scene, char *line)
{
	char	**tokens;
	bool	result;
	int		valid_token_count;
	int		i;

	printf("Parsing line: '%s'\n", line);
	tokens = ft_split(line, ' ');
	if (!tokens)
	{
		*error() = ERR_MEM;
		return (false);
	}
	valid_token_count = 0;
	i = 0;
	while(tokens[i])
	{
		if (tokens[i][0] != '\0')
			tokens[valid_token_count++] = tokens[i];
		else
			free(tokens[i]);
		i++;
	}
	tokens[valid_token_count] = NULL;
	if (valid_token_count == 0)
	{
		free(tokens);
		return (true);
	}
	result = parse_scene_element(tokens, scene);
	free_tokens(tokens);
	return (result);
}

static bool	parse_file_lines(t_scene *scene, int fd)
{
	char	*line;
	char	*trimmed_line;
	bool	result;

	result = true;
	while (true)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		trimmed_line = ft_strtrim(line, " \t\n\r");
		free(line);
		if (!trimmed_line)
			continue ;
		if (trimmed_line[0] == '\0' || trimmed_line[0] == '#')
		{
			free(trimmed_line);
			continue ;
		}
		if (!parse_line(scene, trimmed_line))
		{
			free(trimmed_line);
			result = false;
			break ;
		}
		free(trimmed_line);
	}
	return (result);
}

bool	parse_map(t_scene *scene, const char *file)
{
	int		fd;
	bool	result;

	if (!vector_init(&scene->objects, sizeof(t_sphere)) ||
		!vector_init(&scene->lights, sizeof(t_light)))
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
	vector_free(&scene->objects, del_objects);
	vector_free(&scene->lights, del_lights);
	return (result);
}
