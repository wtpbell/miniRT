/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   light.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/14 12:05:04 by bewong        #+#    #+#                 */
/*   Updated: 2025/06/22 10:43:36 by jboon         ########   odam.nl         */
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

static bool	parse_spot_light(char **tokens, t_scene *scene)
{
	const t_v2f	lim_ang = init_v2f(0.0f, 180.0f);
	t_light		*spot_light;

	spot_light = ft_calloc(1, sizeof(t_light));
	if (!spot_light)
		return (perror("parse_spot_light"), false);
	if (!parse_v3f(&spot_light->pos, tokens[1])
		|| !parse_dir(&spot_light->spot.dir, tokens[2])
		|| !parse_light_ratio(&spot_light->intensity, tokens[3])
		|| !parse_float(&spot_light->spot.inner, tokens[4], lim_ang, tokens[4])
		|| !parse_float(&spot_light->spot.outer, tokens[5], lim_ang, tokens[5])
		|| !parse_col(&spot_light->color, tokens[6]))
		return (free(spot_light), false);
	spot_light->type = LIGHT_SPOT;
	spot_light->spot.inner = cosf(spot_light->spot.inner * 0.5f * DEGTORAD);
	spot_light->spot.outer = cosf(spot_light->spot.outer * 0.5f * DEGTORAD);
	if (!vector_add(&scene->lights, spot_light))
		return (free(spot_light), false);
	return (true);
}

bool	parse_light(char **tokens, t_scene *scene)
{
	return ((ft_strcmp(*tokens, "A") == 0 && parse_ambient_light(tokens, scene))
		|| (ft_strcmp(*tokens, "L") == 0 && parse_point_light(tokens, scene))
		|| (ft_strcmp(*tokens, "spl") == 0 && parse_spot_light(tokens, scene)));
}
