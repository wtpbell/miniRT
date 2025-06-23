/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   element_parser.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 16:23:01 by bewong        #+#    #+#                 */
/*   Updated: 2025/06/23 23:41:10 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

#define EL_SIZE	11

static const t_ele	g_parsers[EL_SIZE] = {
	{"C", SCENE_CAMERA, 4, -1, parse_camera},
	{"A", SCENE_AMBIENT, 3, 3, parse_ambient_light},
	{"L", SCENE_POINT_LIGHT, 4, 4, parse_point_light},
	{"l", SCENE_NONE, 4, 4, parse_point_light},
	{"spl", SCENE_NONE, 7, 7, parse_spot_light},
	{"sp", SCENE_NONE, 4, 5, parse_sphere},
	{"pl", SCENE_NONE, 4, 5, parse_plane},
	{"tri", SCENE_NONE, 5, 6, parse_triangle},
	{"cy", SCENE_NONE, 6, 7, parse_cylinder},
	{"co", SCENE_NONE, 6, 7, parse_cone},
	[EL_SIZE - 1] = {"m_", SCENE_NONE, 2, -1, parse_material}
};

static bool	flag_available(t_scene_flags sc_flag, t_scene_flags el_flag)
{
	return (el_flag == SCENE_NONE || (sc_flag & el_flag) != el_flag);
}

t_parser	element_parser(char **tokens, t_scene *scene, const char *line)
{
	int	i;
	int	count;

	i = 0;
	while (i < EL_SIZE)
	{
		if ((i == (EL_SIZE - 1) && ft_strncmp(g_parsers[i].spec, *tokens, 2) == 0)
			|| ft_strcmp(g_parsers[i].spec, *tokens) == 0)
		{
			if (!flag_available(scene->scene_flags, g_parsers->sc_flag))
				return (print_error(ERR_DUPLICATE, g_parsers->spec, line), NULL);
			count = (int)token_count(tokens);
			if (count < g_parsers[i].min_cnt
				|| (g_parsers->max_cnt != -1 && count > g_parsers[i].max_cnt))
				return (print_error(ERR_TOKEN_COUNT, "token", line), NULL);
			return (g_parsers->parse_fn);
		}
		++i;
	}
	return (print_error(ERR_UNKNOWN_TOKEN, "token", *tokens), NULL);
}
