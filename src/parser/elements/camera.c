/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   camera.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/14 12:05:02 by bewong        #+#    #+#                 */
/*   Updated: 2025/06/23 23:34:27 by jboon         ########   odam.nl         */
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

	fields[0] = init_field("ap", &cam->aperture, FIELD_FLT,
			(t_v2f){.x = 0.0f, .y = FLT_MAX});
	fields[1] = init_field("fc", &cam->focus_dist, FIELD_FLT,
			(t_v2f){.x = 0.1f, .y = FLT_MAX});
	return (parse_fields(fields, 2, tokens + CAMERA_MIN_TOKENS));
}

static void	camera_init(t_cam *cam, t_v3f pos, t_v3f dir, float fov)
{
	cam->t.pos = pos;
	dir.z = -dir.z;
	cam->t.dir = v3f_norm(dir);
	if (fabsf(dir.y) > 0.999f)
		cam->t.up = v3f_scale(g_v3f_back, ft_signf(dir.y));
	else
		cam->t.up = v3f_norm(v3f_cross(v3f_cross(dir, g_v3f_up), dir));
	id_m4x4(cam->view_matrix);
	cam->fov = fov;
	cam->aperture = 0.0f;
	cam->focus_dist = 10.0f;
}

bool	parse_camera(char **tokens, t_scene *scene)
{
	t_v3f	pos;
	t_v3f	dir;
	float	fov;

	if (!parse_v3f(&pos, tokens[1]) || !parse_dir(&dir, tokens[2])
		|| !parse_fov(&fov, tokens[3]))
		return (false);
	ft_bzero(&scene->camera, sizeof(t_cam));
	scene->scene_flags |= SCENE_CAMERA;
	camera_init(&scene->camera, pos, dir, fov);
	if (!parse_camera_fields(&scene->camera, tokens))
		return (false);
	if (scene->camera.focus_dist <= 0.0f)
		return (print_error(
				ERR_RANGE, "Focus distance must be > 0.0", ""), false);
	return (true);
}
