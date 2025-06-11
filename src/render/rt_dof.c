/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_dof.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/14 12:05:02 by bewong        #+#    #+#                 */
/*   Updated: 2025/06/11 15:34:41 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "scene.h"
#include "vector.h"
#include "rt_math.h"
#include "random.h"
#include "matrix.h"
#include "minirt.h"
#include <stdio.h>
#include <math.h>

void	update_camera_view(t_cam *cam)
{
	float		theta;
	float		half_height;
	float		half_width;
	t_v3f		focal_center;

	if (cam->focus_dist <= 0.0f)
		cam->focus_dist = 10.0f; // Default focus distance
	view_matrix(cam->view_matrix, cam->t.pos, cam->t.dir, cam->t.up);
	cam->w = v3f_scale(cam->t.dir, -1.0f);  // Forward is negative Z, away from camera
	cam->u = v3f_norm(v3f_cross(cam->t.up, cam->w));  // Right
	cam->v = v3f_norm(v3f_cross(cam->w, cam->u));     // Up 
	theta = cam->fov * M_PI / 180.0f; // FOV -> radians
	half_height = tanf(theta * 0.5f) * cam->focus_dist;
	half_width = half_height * cam->aspect_ratio;
	cam->horizontal = v3f_scale(cam->u, 2.0f * half_width);
	cam->vertical = v3f_scale(cam->v, 2.0f * half_height);
	focal_center = v3f_add(cam->t.pos, v3f_scale(cam->w, -cam->focus_dist));
	cam->lower_left = v3f_sub(
		v3f_sub(focal_center, v3f_scale(cam->horizontal, 0.5f)),
		v3f_scale(cam->vertical, 0.5f)
	);
	printf("\nCamera View Setup:");
	printf("\n  Position: (%.2f, %.2f, %.2f)", cam->t.pos.x, cam->t.pos.y, cam->t.pos.z);
	printf("\n  Direction: (%.2f, %.2f, %.2f)", cam->t.dir.x, cam->t.dir.y, cam->t.dir.z);
	printf("\n  Focus distance: %.2f", cam->focus_dist);
	printf("\n  Aperture: %.4f\n", cam->aperture);
}

static void	concentric_sample_disk(float u1, float u2, t_v2f *lens)
{
	float	sx;
	float	sy;
	float	r;
	float	theta;

	sx = 2.0f * u1 - 1.0f;
	sy = 2.0f * u2 - 1.0f;
	if (sx == 0 && sy == 0)
	{
		init_v2f(lens->x, lens->y);
		return ;
	}
	if (fabsf(sx) > fabsf(sy))
	{
		r = sx;
		theta = (M_PI / 4.0f) * (sy / sx);
	}
	else
	{
		r = sy;
		theta = (M_PI / 2.0f) - (M_PI / 4.0f) * (sx / sy);
	}
	lens->x = r * cosf(theta);
	lens->y = r * sinf(theta);
}


t_ray	get_ray_with_dof(t_cam *cam, float u, float v)
{
	t_v3f	viewport_point;
	t_v3f	ray_dir;
	t_v3f	focal_point;
	t_v3f	origin;
	t_v2f	lens;

	viewport_point = v3f_add(cam->lower_left,
		v3f_add(v3f_scale(cam->horizontal, u), v3f_scale(cam->vertical, v)));
	ray_dir = v3f_norm(v3f_sub(viewport_point, cam->t.pos));
	focal_point = v3f_add(cam->t.pos, v3f_scale(ray_dir, cam->focus_dist));
	concentric_sample_disk(frandom(), frandom(), &lens);
	lens.x *= 1.0f - sqrtf(lens.x * lens.x + lens.y * lens.y);
	lens.y *= 1.0f - sqrtf(lens.x * lens.x + lens.y * lens.y);
	lens.x *= cam->aperture * 0.5f;
	lens.y *= cam->aperture * 0.5f;
	origin = v3f_add(cam->t.pos,
		v3f_add(v3f_scale(cam->u, lens.x), v3f_scale(cam->v, lens.y)));
	focal_point = v3f_add(cam->lower_left,
		v3f_add(v3f_scale(cam->horizontal, u), v3f_scale(cam->vertical, v)));
	return (t_ray){
		.origin = origin,
		.direction = v3f_norm(v3f_sub(focal_point, origin))};
}

// weight = expf(-r * r * k); // k = sharpness, e.g. 2.0f
// weight = powf(1.0f - r, 2);
