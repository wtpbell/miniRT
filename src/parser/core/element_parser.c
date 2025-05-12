/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   element_parser.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 16:23:01 by bewong        #+#    #+#                 */
/*   Updated: 2025/05/12 17:20:12 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static bool (*get_parser(const char *type))(char **a, t_scene *n)
{
	if (!type)
		return (NULL);
	if (ft_strcmp(type, "sp") == 0)
		return (parse_sphere);
	// else if (ft_strcmp(type, "pl") == 0)
	// 	return (parse_plane);
	// else if (ft_strcmp(type, "cy") == 0)
	// 	return (parse_cylinder);
	// else if (ft_strcmp(type, "l") == 0)
	// 	return (parse_light);
	// else if (ft_strcmp(type, "c") == 0)
	// 	return (parse_camera);
	return (NULL);
}


bool	parse_scene_element(const char *type, char **tokens, t_scene *scene)
{
	bool	(*parser)(char **, t_scene *);
	parser = get_parser(type);
	if (!parser)
	{
		*error() = ERR_UNKNOWN_TOKEN;
		return (false);
	}
	return (parser(tokens, scene));
}



// static bool parse_plane(char **tokens, t_scene *scene)
// {
// 	(void)tokens;
// 	(void)scene;
// 	return (false);
// }

// static bool parse_cylinder(char **tokens, t_scene *scene)
// {
// 	(void)tokens;
// 	(void)scene;
// 	return (false);
// }

// static bool parse_light(char **tokens, t_scene *scene)
// {
// 	(void)tokens;
// 	(void)scene;
// 	return (false);
// }

// static bool parse_camera(char **tokens, t_scene *scene)
// {
// 	(void)tokens;
// 	(void)scene;
// 	return (false);
// }
