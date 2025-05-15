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

bool	validate_and_norm_dir(t_v3f *dir, const char *str)
{
	if (!v3f_dir_valid(dir))
	{
		print_error(ERR_RANGE, "direction range -1.0f - 1.0f", str);
		return (false);
	}
	*dir = v3f_norm(*dir);
	return (true);
}

bool	v3f_dir_valid(t_v3f *dir)
{
	return (dir->x >= -1.0f && dir->x <= 1.0f
		&& dir->y >= -1.0f && dir->y <= 1.0f
		&& dir->z >= -1.0f && dir->z <= 1.0f);
}

bool	validate_scene(const t_scene *scene)
{
	if (!(scene->scene_flags & SCENE_AMBIENT))
		return (print_error(ERR_MISSING_COMPONENT, "Missing ambient light (A)", NULL), false);
	if (!(scene->scene_flags & SCENE_POINT_LIGHT))
		return (print_error(ERR_MISSING_COMPONENT, "Missing point light (L)", NULL), false);
	return (true);
}
