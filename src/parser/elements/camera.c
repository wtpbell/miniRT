/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 12:05:02 by bewong            #+#    #+#             */
/*   Updated: 2025/05/14 12:05:02 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static bool	parse_fov(float *fov, const char *str)
{
	if (!ft_stof(str, fov))
		return (false);
	if (*fov < MIN_FOV || *fov > MAX_FOV)
	{
		print_error(ERR_RANGE, "FOV range 0-180", str);
		return (false);
	}
	return (true);
}

bool	parse_camera(char **tokens, t_scene *scene)
{
	t_v3f		pos;
	t_v3f		dir;
	float		fov;

	if (scene->scene_flags & SCENE_CAMERA)
	{
		print_error(ERR_DUPLICATE, "Camera", "Multiple cameras not allowed");
		return (false);
	}
	if (!parse_v3f(&pos, tokens[1]) || !parse_dir(&dir, tokens[2])
		|| !parse_fov(&fov, tokens[3]))
		return (false);
	ft_bzero(&scene->camera, sizeof(t_camera));
	scene->camera.t.pos = pos;
	scene->camera.t.dir = dir;
	scene->camera.fov = fov;
	scene->scene_flags |= SCENE_CAMERA;
	return (true);
}
