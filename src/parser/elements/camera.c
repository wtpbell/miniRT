/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 12:05:02 by bewong            #+#    #+#             */
/*   Updated: 2025/06/04 20:21:43 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <string.h>
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
		up = (t_v3f){.x = 0.0f, .y = 0.0f, .z = -1.0f + 2.0f * (dir.y < 0.0f)};
	else
		up = v3f_norm(v3f_cross(v3f_cross(
					dir, (t_v3f){.x = 0.0f, .y = 1.0f, .z = 0.0f}), dir));
	scene->camera.t.up = up;
	scene->camera.fov = fov;
	// Only set default DoF values if they haven't been set yet
	if (scene->camera.aperture <= 0.0f)
		scene->camera.aperture = 0.01f;
	if (scene->camera.focus_dist <= 0.0f)
		scene->camera.focus_dist = 10.0f;
	id_m4x4(scene->camera.view_matrix);
	scene->camera.view_matrix[15] = 1.0f;
	printf("Camera DoF settings:\n");
	printf("  Aperture: %.4f\n", scene->camera.aperture);
	printf("  Focus distance: %.2f\n", scene->camera.focus_dist);
	printf("  Camera up vector: (%.2f, %.2f, %.2f)\n", up.x, up.y, up.z);
	scene->scene_flags |= SCENE_CAMERA;
	return (true);
}
