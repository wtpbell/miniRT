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
	if (!scene->lights)
	{
		scene->lights = malloc(sizeof(t_light));
		if (!scene->lights)
			return (perror("Ambient light allocation failed"), false);
	}
	if (!parse_light_ratio(&scene->lights->intensity, tokens[1])
					|| !parse_col(&scene->lights->col, tokens[2]))
		return (false);
	scene->lights->type = LIGHT_AMBIENT;
	scene->lights->pos = (t_v3f){{0, 0, 0}};
	scene->ambient_light_set = true;
	return (true);
}

static bool	parse_point_light(char **tokens, t_scene *scene)
{
	if (!scene->lights)
	{
		scene->lights = malloc(sizeof(t_light));
		if (!scene->lights)
			return (perror("Point light allocation failed"), false);
	}
	if (!parse_v3f(&scene->lights->pos, tokens[1])
		|| !parse_light_ratio(&scene->lights->intensity, tokens[2])
		|| !parse_col(&scene->lights->col, tokens[3]))
		return (false);
	scene->lights->type = LIGHT_POINT;
	scene->light_set = true;
	return (true);
}

bool	parse_light(char **tokens, t_scene *scene)
{
	if (scene->ambient_light_set && ft_strcmp(tokens[0], "A") == 0)
		return (print_error(ERR_DUPLICATE, "ambient light", tokens[0]), false);
	if (scene->light_set && ft_strcmp(tokens[0], "L") == 0)
		return (print_error(ERR_DUPLICATE, "point light", tokens[0]), false);
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
