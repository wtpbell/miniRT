/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   render.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/10 17:15:02 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/22 17:06:12 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <stdio.h>
#include <stdint.h>

#include "MLX42/MLX42.h"
#include "libft.h"
#include "minirt.h"
#include "scene.h"
#include "color.h"
#include "rt_math.h"
#include "container.h"

#include "debug/rt_debug.h"

#define MAX_DEPTH	5

void	init_object_matrices(t_obj *obj)
{
	t_v3f	up;

	up = (t_v3f){.x = 0, .y = 1, .z = 0};
	if (obj->t.up.x != 0 || obj->t.up.y != 0 || obj->t.up.z != 0)
		up = obj->t.up;
	obj_to_world(obj->to_world, obj->t.pos, obj->t.dir, up);
	invert_m4x4(obj->to_obj, obj->to_world);
}

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
	shadow_ray.origin = v3f_add(ray->origin, v3f_scale(ray->direction, t));
	shadow_ray.direction = hit->calc_norm(hit, shadow_ray.origin);
	// TODO: Find correct color to return
	if (depth == MAX_DEPTH)
		return (normal_color(shadow_ray.direction));
	// TODO: Do fancy stuff;
	// call trace (depth + 1)
	return (normal_color(shadow_ray.direction));
	// return (hit->r.col); //(the real color from test file)
}

static void	compute_ray(uint32_t x, uint32_t y, t_cam *cam, t_ray *ray)
{
	t_v3f	camera_space;
	float	view;

	view = tanf(cam->fov * 0.5f * DEGTORAD);
	camera_space.x = (2.0f * ((x + 0.5f) / cam->img_plane->width) - 1.0f) * cam->aspect_ratio * view;
	camera_space.y = (1.0f - 2.0f * ((y + 0.5f) / cam->img_plane->height)) * view;
	camera_space.z = -1.0f;
	ray->direction.x = cam->view_matrix[0] * camera_space.x +
						cam->view_matrix[4] * camera_space.y +
						cam->view_matrix[8] * camera_space.z;
	ray->direction.y = cam->view_matrix[1] * camera_space.x +
						cam->view_matrix[5] * camera_space.y +
						cam->view_matrix[9] * camera_space.z;
	ray->direction.z = cam->view_matrix[2] * camera_space.x +
						cam->view_matrix[6] * camera_space.y +
						cam->view_matrix[10] * camera_space.z;
	ray->direction = v3f_norm(ray->direction);
}

void	render(t_scene *scene)
{
	uint32_t	x;
	uint32_t	y;
	t_ray		ray;
	mlx_image_t	*img;

	y = 0;
	img = scene->camera.img_plane;
	ray.origin = scene->camera.t.pos;// mul_v3_m4x4(init_v3f(0, 0, 0), scene->camera.view_matrix);
	while (y < img->height)
	{
		x = 0;
		while (x < img->width)
		{
			compute_ray(x, y, &scene->camera, &ray);
			mlx_put_pixel(img, x, y, trace(&ray, scene, 0));
			++x;
		}
		++y;
	}
}
