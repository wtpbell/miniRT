/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 12:05:04 by bewong            #+#    #+#             */
/*   Updated: 2025/05/14 12:05:04 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static bool	parse_light_ratio(float *ratio, const char *str)
{
	t_v2f	light_ratio_range;

	light_ratio_range = init_v2f(MIN_LIGHT_RATIO, MAX_LIGHT_RATIO);
	return (parse_float(ratio, str, light_ratio_range,
			"parse light ratio"));
}

static bool	parse_ambient_light(char **tokens, t_scene *scene)
{
	t_light	*ambient_light;

	if (scene->scene_flags & SCENE_AMBIENT)
		return (print_error(ERR_DUPLICATE, "ambient light", tokens[0]), false);
	ambient_light = ft_calloc(1, sizeof(t_light));
	if (!ambient_light)
		return (perror("parse_ambient_light"), false);
	if (!parse_light_ratio(&ambient_light->intensity, tokens[1])
		|| !parse_col(&ambient_light->color, tokens[2]))
		return (free(ambient_light), false);
	ambient_light->type = LIGHT_AMBIENT;
	if (!vector_add(&scene->lights, ambient_light))
		return (free(ambient_light), false);
	scene->scene_flags |= SCENE_AMBIENT;
	return (true);
}

static bool	parse_point_light(char **tokens, t_scene *scene)
{
	t_light	*point_light;

	if (scene->scene_flags & SCENE_POINT_LIGHT)
		return (print_error(ERR_DUPLICATE, "point light", tokens[0]), false);
	point_light = ft_calloc(1, sizeof(t_light));
	if (!point_light)
		return (perror("parse_point_light"), false);
	if (!parse_v3f(&point_light->pos, tokens[1])
		|| !parse_light_ratio(&point_light->intensity, tokens[2])
		|| !parse_col(&point_light->color, tokens[3]))
		return (free(point_light), false);
	point_light->type = LIGHT_POINT;
	if (!vector_add(&scene->lights, point_light))
		return (free(point_light), false);
	scene->scene_flags |= SCENE_POINT_LIGHT;
	return (true);
}

bool	parse_light(char **tokens, t_scene *scene)
{
	return ((ft_strcmp(*tokens, "A") == 0 && parse_ambient_light(tokens, scene))
		|| (ft_strcmp(*tokens, "L") == 0 && parse_point_light(tokens, scene)));
}
