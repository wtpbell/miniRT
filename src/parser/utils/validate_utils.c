/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 18:55:49 by bewong            #+#    #+#             */
/*   Updated: 2025/05/14 18:55:49 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

bool	validate_scene(const t_scene *scene)
{
	if (!(scene->scene_flags & SCENE_CAMERA))
		return (rt_error(ERR_MISSING_COMPONENT,
				"Missing camera (C)", NULL), false);
	if (!(scene->scene_flags & SCENE_POINT_LIGHT))
		return (rt_error(ERR_MISSING_COMPONENT,
				"Missing point light (L)", NULL), false);
	if (!(scene->scene_flags & SCENE_AMBIENT))
		return (rt_error(ERR_MISSING_COMPONENT,
				"Missing ambient light (A)", NULL), false);
	return (true);
}

bool	split_and_validate(const char *str, char ***out_tokens,
	size_t expected_count, const char *ctx)
{
	*out_tokens = ft_split(str, ',');
	if (!*out_tokens)
		return (sys_error("split_and_validate"), false);
	if (token_count(*out_tokens) != expected_count)
	{
		rt_error(ERR_TOKEN_COUNT, ctx, str);
		free_tokens(*out_tokens);
		return (false);
	}
	return (true);
}

bool	parse_float(float *out, const char *str,
	t_v2f range, const char *token)
{
	if (!ft_stof(str, out))
	{
		rt_error(ERR_STOF, token, str);
		return (false);
	}
	if (*out < range.x || *out > range.y)
	{
		rt_error(ERR_RANGE, token, str);
		return (false);
	}
	return (true);
}

bool	parse_int(int *out, const char *str,
	t_v2f range, const char *token)
{
	if (!ft_stoi(str, out))
	{
		rt_error(ERR_STOI, token, str);
		return (false);
	}
	if (*out < (int)range.x || *out > (int)range.y)
	{
		rt_error(ERR_RANGE, token, str);
		return (false);
	}
	return (true);
}
