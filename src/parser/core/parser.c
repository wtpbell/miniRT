/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 16:23:01 by bewong        #+#    #+#                 */
/*   Updated: 2025/06/23 19:06:11 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

#define EL_SIZE	10

bool	flag_available(t_scene_flags sc_flag, t_scene_flags el_flag)
{
	return (el_flag == SCENE_NONE || (sc_flag & el_flag) != el_flag);
}

void	*parse_scene_element_2(char **tokens, t_scene *scene, const char *line)
{
	int			i;
	size_t		count;
	const int	i_mat = 9;
	const t_ele	elements[EL_SIZE] = {
		{"C", SCENE_CAMERA, 4, -1, parse_camera},
		{"A", SCENE_AMBIENT, 3, 3, parse_ambient_light}, // should be parse_ambient
		{"L", SCENE_POINT_LIGHT, 4, 4, parse_point_light},
		{"l", SCENE_NONE, 4, 4, parse_point_light},
		{"spl", SCENE_NONE, 7, 7, parse_spot_light},
		{"sp", SCENE_NONE, 4, 5, parse_sphere},
		{"pl", SCENE_NONE, 4, 5, parse_plane},
		{"tri", SCENE_NONE, 5, 6, parse_triangle},
		{"cy", SCENE_NONE, 6, 7, parse_cylinder},
		{"co", SCENE_NONE, 6, 7, parse_cone},
		[i_mat] = {"m_", SCENE_NONE, 2, -1, parse_material} // uses strncmp instead...
	};

	i = 0;
	while (i < EL_SIZE)
	{
		if ((i == i_mat && ft_strncmp(elements[i].spec, *tokens, 2))
			|| ft_strcmp(elements[i].spec, *tokens))
		{
			if (flag_available(scene->scene_flags, elements->sc_flag))
			{
				count = token_count(tokens);
				if (count >= elements[i].min_cnt
					&& (elements->max_cnt == -1 || count <= elements->max_cnt))
					return (elements->parse_fn);
				return (print_error(ERR_TOKEN_COUNT, "token", line), NULL);
			}
			// Duplicate!
			print_error(ERR_DUPLICATE, elements->spec, "Multiple cameras not allowed");
			return (NULL);
		}
		++i;
	}
	return (print_error(ERR_UNKNOWN_TOKEN, "token", *tokens), NULL);
}

static bool	parse_line(t_scene *scene, char *line)
{
	char	**tokens;
	bool	result;

	while (*line && ft_strchr(" \f\n\r\t\v", *line))
		++line;
	if (*line == '\0' || *line == '#')
		return (true);
	clean_spaces(line);
	if (!validate_commas(line))
		return (false);
	tokens = ft_split(line, ' ');
	if (!tokens)
		return (perror("parse_line"), false);
	if (!validate_tokens(*tokens, line))
		return (free_tokens(tokens), false);
	result = parse_scene_element(*tokens, tokens, scene);
	free_tokens(tokens);
	return (result);
}

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
		perror("parse_map");
		cleanup_scene(scene);
		return (false);
	}
	result = parse_file_lines(scene, fd);
	close(fd);
	if (!result)
		cleanup_scene(scene);
	return (result);
}
