/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 16:23:01 by bewong            #+#    #+#             */
/*   Updated: 2025/06/10 15:22:25 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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
