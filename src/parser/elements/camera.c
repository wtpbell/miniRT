/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 12:05:02 by bewong            #+#    #+#             */
/*   Updated: 2025/06/13 19:21:12 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "rt_math.h"
#include "parser.h"

#define CAMERA_MIN_TOKENS 4

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

static bool	parse_camera_fields(t_cam *cam, char **tokens)
{
	t_field		fields[2];

	fields[0] = init_field("ap", &cam->aperture, FIELD_FLOAT,
		(t_v2f){.x = 0.0f, .y = FLT_MAX});
	fields[1] = init_field("fc", &cam->focus_dist, FIELD_FLOAT,
		(t_v2f){.x = 0.1f, .y = FLT_MAX});
	return (parse_fields(fields, 2, tokens + CAMERA_MIN_TOKENS));
}

bool	parse_camera(char **tokens, t_scene *scene)
{
	t_v3f	pos;
	t_v3f	dir;
	float	fov;
	t_v3f	up;

	if (scene->scene_flags & SCENE_CAMERA)
	{
		print_error(ERR_DUPLICATE, "Camera", "Multiple cameras not allowed");
		return (false);
	}
	if (!parse_v3f(&pos, tokens[1]) || !parse_dir(&dir, tokens[2]) || 
		!parse_fov(&fov, tokens[3]))
		return (false);
	ft_bzero(&scene->camera, sizeof(t_cam));
	scene->camera.aperture = 0.0f;
	scene->camera.focus_dist = 10.0f;
	if (!parse_camera_fields(&scene->camera, tokens))
		return (false);
	if (scene->camera.focus_dist <= 0.0f)
		return (print_error(ERR_RANGE, "Focus distance must be > 0", ""),false);
	scene->camera.t.pos = pos;
	dir.z = -dir.z;
	scene->camera.t.dir = v3f_norm(dir);
	if (fabsf(dir.y) > 0.999f)
		up = v3f_scale(g_v3f_back, ft_signf(dir.y));
	else
		up = v3f_norm(v3f_cross(v3f_cross(dir, g_v3f_up), dir));
	scene->camera.t.up = up;
	scene->camera.fov = fov;
	id_m4x4(scene->camera.view_matrix);
	scene->scene_flags |= SCENE_CAMERA;
	return (true);
}
