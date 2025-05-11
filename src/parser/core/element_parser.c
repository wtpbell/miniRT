/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   element_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 16:23:01 by bewong            #+#    #+#             */
/*   Updated: 2025/05/11 18:19:14 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

typedef bool (*parse_func)(char **, t_scene *);

bool	parse_sphere(char **tokens, t_scene *scene);
// bool	parse_plane(char **tokens, t_scene *scene);
// bool	parse_cylinder(char **tokens, t_scene *scene);
// bool	parse_light(char **tokens, t_scene *scene);
// bool	parse_camera(char **tokens, t_scene *scene);

// Lookup table for all scene element parsers
static const struct {
    const char *type;
    parse_func parser;
} parsers[] = {
    {"sp", parse_sphere},
    // {"pl", parse_plane},
    // {"cy", parse_cylinder},
    // {"l", parse_light},
    // {"c", parse_camera},
    {NULL, NULL}
};

bool	parse_scene_element(const char *type, char **tokens, t_scene *scene)
{
	size_t	i;

	i = 0;
	while (parsers[i].type != NULL)
	{
		if (ft_strcmp(type, parsers[i].type) == 0)
			return (parsers[i].parser(tokens, scene));
		i++;
	}
	*error() = ERR_UNKNOWN_TOKEN;
	return (false);
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
