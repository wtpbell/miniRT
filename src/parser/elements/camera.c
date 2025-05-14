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

static t_camera	*create_camera(t_camera *camera, t_v3f pos, t_v3f dir, float fov)
{
	camera->t.pos = pos;
	camera->t.dir = dir;
	camera->fov = fov;
	return (camera);
}

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
	t_camera	*camera;
	t_v3f		pos;
	t_v3f		dir;
	float		fov;

	if (scene->camera_set)
	{
		print_error(ERR_DUPLICATE, "Camera", "Multiple cameras not allowed");
		return (false);
	}
	if (!parse_v3f(&pos, tokens[1]) || !parse_dir(&dir, tokens[2]) 
	|| !parse_fov(&fov, tokens[3]))
		return (false);
	camera = malloc(sizeof(t_camera));
	if (!camera)
		return (perror("Camera allocation failed"), false);
	camera = create_camera(camera, pos, dir, fov);
	if (!camera)
		return (false);
	scene->camera = *camera;
	scene->camera_set = true;
	free(camera);
	return (true);
}
