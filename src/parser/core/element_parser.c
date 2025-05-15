/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   element_parser.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 16:23:01 by bewong        #+#    #+#                 */
/*   Updated: 2025/05/15 12:08:07 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static t_parser	get_parser(const char *type)
{
	if (!type)
		return (NULL);
	if (ft_strcmp(type, "sp") == 0)
		return (parse_sphere);
	else if (ft_strcmp(type, "pl") == 0)
		return (parse_plane);
	else if (ft_strcmp(type, "cy") == 0)
		return (parse_cylinder);
	else if (ft_strcmp(type, "L") == 0 || ft_strcmp(type, "A") == 0)
		return (parse_light);
	else if (ft_strcmp(type, "C") == 0)
		return (parse_camera);
	return (NULL);
}

bool	parse_scene_element(const char *type, char **tokens, t_scene *scene)
{
	t_parser	parser;

	parser = get_parser(type);
	if (!parser)
	{
		print_error(ERR_UNKNOWN_TOKEN, "token", *tokens);
		return (false);
	}
	return (parser(tokens, scene));
}
