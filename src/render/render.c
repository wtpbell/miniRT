/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   render.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/10 17:15:02 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/20 19:06:25 by bewong        ########   odam.nl         */
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

#define MAX_DEPTH	5

static	t_col32	normal_color(t_v3f norm)
{
	norm = v3f_scale(v3f_add(norm, init_v3f(1.0f, 1.0f, 1.0f)), 255 >> 1);
	return (init_col32(norm.x, norm.y, norm.z, 255));
}

static t_col32	gradient_color(t_v3f dir, t_col32 b)
{

	(void)b;
	return (normal_color(dir));
	// hitcolor = v3f_scale(v3f_add(dir,
	// // 			v3f_norm(init_v3f(get_r(b), get_g(b), get_b(b)))), 255 >> 1);
	// return (init_col32(hitcolor.x * (get_r(b) / 255.0f), hitcolor.y * (get_g(b) / 255.0f), hitcolor.z * (get_b(b) / 255.0f), 255));
	// return (init_col32(ft_absf(dir.y) * get_r(b), ft_absf(dir.x) * get_g(b), ft_absf(dir.x) * get_b(b), 255));	
}

static t_obj	*find_intersection(t_ray *ray, t_scene *scene, float *t)
{
	int		i;
	float	dst;
	t_obj	*obj;
	t_obj	*hit;

	i = 0;
	hit = NULL;
	*t = FLT_MAX;
	while (i < scene->objects.size)
	{
		obj = (t_obj *)scene->objects.items[i];
		if (obj->intersect(obj, ray, &dst) && dst < *t)
		{
			*t = dst;
			hit = obj;
		}
		++i;
	}
	return (hit);
}

static t_col32	trace(t_ray *ray, t_scene *scene, uint32_t depth)
{
	float	t;
	t_obj	*hit;
	t_ray	shadow_ray;

	hit = find_intersection(ray, scene, &t);
	if (hit == NULL)
		return (gradient_color(ray->direction, scene->camera.bg_col));
	else
	{
		shadow_ray.origin = v3f_add(ray->origin, v3f_scale(ray->direction, t));
		shadow_ray.direction = hit->calc_norm(hit, shadow_ray.origin);
		// TODO: Find correct color to return
		if (depth == MAX_DEPTH)
			return (normal_color(shadow_ray.direction));
		// TODO: Do fancy stuff;
		// call trace (depth + 1)
		return (normal_color(shadow_ray.direction));
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
	ray.origin = scene->camera.t.pos;// mul_v3_m4x4(init_v3f(0, 0, 0), scene->camera.cam_to_world);
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
