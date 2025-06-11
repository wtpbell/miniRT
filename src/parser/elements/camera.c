/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   camera.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/14 12:05:02 by bewong        #+#    #+#                 */
/*   Updated: 2025/06/11 14:37:46 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "rt_math.h"
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
	t_v3f		up;
	float		fov;

	if (scene->scene_flags & SCENE_CAMERA)
	{
		print_error(ERR_DUPLICATE, "Camera", "Multiple cameras not allowed");
		return (false);
	}
	if (!parse_v3f(&pos, tokens[1]) || !parse_dir(&dir, tokens[2])
		|| !parse_fov(&fov, tokens[3]))
		return (false);
	ft_bzero(&scene->camera, sizeof(t_cam));
	scene->camera.t.pos = pos;
	dir.z = -dir.z;
	scene->camera.t.dir = v3f_norm(dir);
	if (fabsf(dir.y) > 0.999f)
		up = v3f_scale(g_v3f_back, ft_signf(dir.y));
	else
		up = v3f_norm(v3f_cross(v3f_cross(
						dir, (t_v3f){.x = 0.0f, .y = 1.0f, .z = 0.0f}), dir));
	scene->camera.t.up = up;
	scene->camera.fov = fov;
	scene->camera.aperture = 0.0f;
	scene->camera.focus_dist = 0.0f;
	id_m4x4(scene->camera.view_matrix);
	scene->camera.view_matrix[15] = 1.0f;
	scene->scene_flags |= SCENE_CAMERA;
	return (true);
}
