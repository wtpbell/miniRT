/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   render.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/10 17:15:02 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/19 11:42:25 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "MLX42/MLX42.h"
#include "libft.h"
#include "minirt.h"
#include "scene.h"
#include "color.h"
#include "rt_math.h"
#include "container.h"

#include "debug/rt_debug.h"

static t_col32	trace(t_ray *ray, t_scene *scene, uint32_t depth)
{
	int			i;
	float		min_dist;
	float		curr_dst;
	t_obj		*obj;
	t_obj		*hit;
	t_ray_hit	curr_hit;
	t_vector	objects;

	(void)depth;
	i = 0;
	hit = NULL;
	min_dist = FLT_MAX;
	objects = scene->objects;
	while (i < objects.size)
	{
		obj = (t_obj *)objects.items[i];
		if (obj->intersect(obj, ray, &curr_dst)
			&& curr_dst < min_dist)
			{
				min_dist = curr_dst;
				hit = obj;
			}
		++i;
	}
	if (hit == NULL)
	{
		t_v3f	hitcolor = v3f_scale(v3f_add(ray->direction, (t_v3f){.x = 1, .y = 1, .z = 1}), 255 >> 1);
		return (init_col32(hitcolor.x, hitcolor.y, hitcolor.z, 255));
	}
	else
	{
		curr_hit.hit = v3f_add(ray->origin, v3f_scale(ray->direction, min_dist));
		if (hit->type == OBJ_PLANE)
			curr_hit.normal = hit->t.dir;
		else
			curr_hit.normal = v3f_norm(v3f_sub(curr_hit.hit, hit->t.pos));
		curr_hit.normal = v3f_scale(v3f_add(curr_hit.normal, init_v3f(1, 1, 1)), 255>>1);
		return (init_col32(curr_hit.normal.x, curr_hit.normal.y, curr_hit.normal.z, 255));
	}
	return (scene->camera.bg_col);
}

static void	compute_ray(uint32_t x, uint32_t y, t_cam *cam, t_ray *ray)
{
	t_v3f	camera_space;
	float	view;

	view = tanf(cam->fov / 2.0f * DEGTORAD);
	camera_space.x = (x + .5f) / cam->img_plane->width;
	camera_space.y = (y + .5f) / cam->img_plane->height;

	camera_space.x = (2 * camera_space.x - 1) * cam->aspect_ratio * view;
	camera_space.y = (1 - (2 * camera_space.y)) * view;
	camera_space.z = -1;

	ray->direction = v3f_norm(camera_space);
}

void	render(t_scene *scene)
{
	uint32_t	x;
	uint32_t	y;
	t_ray		ray;
	mlx_image_t	*img;

	y = 0;
	img = scene->camera.img_plane;
	ray.origin = mul_v3_m4x4(init_v3f(0, 0, 0), scene->camera.cam_to_world);
	while (y < img->height)
	{
		x = 0;
		while (x < img->width)
		{
			compute_ray(x, y, &scene->camera, &ray);
			ray.direction = v3f_norm(
				mul_dir_m4x4(ray.direction, scene->camera.cam_to_world));
			mlx_put_pixel(img, x, y, trace(&ray, scene, 0));
			++x;
		}
		++y;
	}
}
