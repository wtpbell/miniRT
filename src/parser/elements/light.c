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

static bool	parse_ambient_light(char **tokens, t_scene *scene)
{
	t_light	*ambient_light;

	if (scene->scene_flags & SCENE_AMBIENT)
		return (print_error(ERR_DUPLICATE, "ambient light", tokens[0]), false);
	scene->scene_flags |= SCENE_AMBIENT;
	ambient_light = malloc(sizeof(t_light));
	if (!ambient_light)
		return (perror("Ambient light allocation failed"), false);
	if (!parse_light_ratio(&ambient_light->intensity, tokens[1])
					|| !parse_col(&ambient_light->col, tokens[2]))
		return (free(ambient_light), false);
	ambient_light->type = LIGHT_AMBIENT;
	ambient_light->pos = (t_v3f){{0, 0, 0}};
	if (!vector_add(&scene->lights, ambient_light))
		return (free(ambient_light), false);
	return (true);
}

static bool	parse_point_light(char **tokens, t_scene *scene)
{
	t_light	*point_light;

	if (scene->scene_flags & SCENE_POINT_LIGHT)
		return (print_error(ERR_DUPLICATE, "point light", tokens[0]), false);
	scene->scene_flags |= SCENE_POINT_LIGHT;
	point_light = malloc(sizeof(t_light));
	if (!point_light)
		return (perror("Point light allocation failed"), false);
	if (!parse_v3f(&point_light->pos, tokens[1])
		|| !parse_light_ratio(&point_light->intensity, tokens[2])
		|| !parse_col(&point_light->col, tokens[3]))
		return (free(point_light), false);
	point_light->type = LIGHT_POINT;
	point_light->pos = (t_v3f){{0, 0, 0}};
	if (!vector_add(&scene->lights, point_light))
	return (free(point_light), false);
	return (true);
}

bool	parse_light(char **tokens, t_scene *scene)
{
	if (ft_strcmp(tokens[0], "A") == 0)
	{
		if (!parse_ambient_light(tokens, scene))
			return (false);
	}
	else if (ft_strcmp(tokens[0], "L") == 0)
	{
		if (!parse_point_light(tokens, scene))
			return (false);
	}
	else
		return (false);
	return (true);
}
