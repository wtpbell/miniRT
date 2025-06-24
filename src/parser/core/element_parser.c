/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   element_parser.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 16:23:01 by bewong        #+#    #+#                 */
/*   Updated: 2025/06/24 10:21:19 by jboon         ########   odam.nl         */
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

static inline bool	is_duplicate(t_scene_flags sc_flag, t_scene_flags flag)
{
	return (flag != SCENE_NONE && (sc_flag & flag) == flag);
}

static inline bool	is_invalid_count(int min, int max, int count)
{
	return (count < min || (max != -1 && count > max));
}

t_parser	element_parser(char **tokens, t_scene *scene, const char *line)
{
	int	i;

	i = 0;
	while (i < EL_SIZE)
	{
		if ((i == (EL_SIZE - 1) && ft_strncmp(g_parsers[i].spec, *tokens, 2) == 0)
			|| ft_strcmp(g_parsers[i].spec, *tokens) == 0)
		{
			if (is_duplicate(scene->scene_flags, g_parsers[i].sc_flag))
				return (print_error(ERR_DUPLICATE, g_parsers[i].spec, line), NULL);
			if (is_invalid_count(g_parsers[i].min_cnt,
								g_parsers[i].max_cnt, (int)token_count(tokens)))
				return (print_error(ERR_TOKEN_COUNT, "token", line), NULL);
			return (g_parsers[i].parse_fn);
		}
		++i;
	}
	return (print_error(ERR_UNKNOWN_TOKEN, "token", *tokens), NULL);
}
