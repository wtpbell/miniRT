/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   render.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/10 17:15:02 by jboon         #+#    #+#                 */
/*   Updated: 2025/06/03 18:35:19 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <stdint.h>
#include <stdio.h>

#include "MLX42/MLX42.h"
#include "libft.h"
#include "minirt.h"
#include "color.h"
#include "scene.h"
#include "rt_math.h"
#include "random.h"
#include "light.h"
#include "material.h"

#define MAX_DEPTH	5

void	init_object_matrices(t_obj *obj)
{
	t_v3f	up;

	up = (t_v3f){.x = 0, .y = 1, .z = 0};
	if (obj->t.up.x != 0 || obj->t.up.y != 0 || obj->t.up.z != 0)
		up = obj->t.up;
	obj_to_world(obj->t.to_world, obj->t.pos, obj->t.dir, up);
	invert_m4x4(obj->t.to_obj, obj->t.to_world);
}

static	t_v3f	normal_color(t_v3f norm)
{
	return (v3f_scale(v3f_add(norm, init_v3f(1.0f, 1.0f, 1.0f)), 0.5f));
}

t_v3f	gradient_color(t_v3f dir, t_v3f b)
{
	(void)b;
	return (normal_color(dir));
}

t_obj	*find_intersection(t_ray *ray, t_scene *scene, float *t)
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
		if (obj->intersect(obj, ray, init_v2f(BIAS, *t), &dst))
		{
			*t = dst;
			hit = obj;
		}
		++i;
	}
	return (hit);
}

static void	init_hit_info(t_ray_hit *hit_info, t_obj *obj, t_ray *ray, float t)
{
	hit_info->hit = v3f_add(ray->origin, v3f_scale(ray->direction, t));
	hit_info->normal = obj->calc_norm(obj, hit_info->hit);
	hit_info->distance = t;
	hit_info->front_face = v3f_dot(ray->direction, hit_info->normal) < 0;
	hit_info->obj = obj;
	if (!hit_info->front_face)
		hit_info->normal = v3f_scale(hit_info->normal, -1.0f);
}

t_v3f	trace(t_ray *ray, t_scene *scene, uint32_t depth)
{
	float		t;
	t_obj		*hit;
	t_ray_hit	hit_info;
	t_v3f		color;

	if (depth <= 0)
		return (init_v3f(0.0f, 0.0f, 0.0f));
	hit = find_intersection(ray, scene, &t);
	if (hit == NULL)
		return (init_v3f(0.0f, 0.0f, 0.0f));
	init_hit_info(&hit_info, hit, ray, t);
	hit_info.ray = ray;
	if (hit->r.mat->type == MAT_LAMBERTIAN)
		color = handle_lambertian(scene, &hit_info);
	else if (hit->r.mat->type == MAT_DIELECTRIC)
		color = handle_dielectric(scene, &hit_info, depth);
	else if (hit->r.mat->type == MAT_METAL)
		color = handle_metal(scene, &hit_info, depth);
	else
		color = (init_v3f(1.0f, 1.0f, 1.0f));
	return (v3f_clampf01(color));
}

static void	compute_ray(float x, float y, t_cam *cam, t_ray *ray)
{
	t_v3f	camera_space;
	float	view;

	view = tanf(cam->fov * 0.5f * DEGTORAD);
	camera_space.x = (2.0f * ((x + 0.5f) / cam->img_plane->width) - 1.0f)
		* cam->aspect_ratio * view;
	camera_space.y = (1.0f - 2.0f * ((y + 0.5f) / cam->img_plane->height))
		* view;
	camera_space.z = -1.0f;
	ray->direction = v3f_norm(mul_dir_m4x4(camera_space, cam->view_matrix));
}

#define SAMPLES_PER_PIXEL 4

static t_v3f	anti_aliasing(t_scene *scene, t_ray *ray,
		uint32_t x, uint32_t y)
{
	const float	size = 1.0f;
	const float	offset = size / 2.0f;
	t_v3f		color;
	t_v3f		final_color;
	uint32_t	s;

	s = 0;
	final_color = g_v3f_zero;
	while (s < SAMPLES_PER_PIXEL)
	{
		compute_ray((float)x + frandom() * size - offset,
			(float)y + frandom() * size - offset, &scene->camera, ray);
		color = trace(ray, scene, MAX_DEPTH);
		final_color = v3f_add(final_color, color);
		++s;
	}
	return (v3f_scale(final_color, 1.0f / (float)SAMPLES_PER_PIXEL));
}

void	render(t_scene *scene)
{
	uint32_t	x;
	uint32_t	y;
	t_ray		ray;
	mlx_image_t	*img;
	t_mat4x4	inv;
	t_v3f		color;

	y = 0;
	img = scene->camera.img_plane;
	invert_m4x4(inv, scene->camera.view_matrix);
	ray.origin = mul_v3_m4x4(g_v3f_zero, inv);
	while (y < img->height)
	{
		x = 0;
		while (x < img->width)
		{
			seed_rand(get_rngstate(x, y, 1));
			color = anti_aliasing(scene, &ray, x, y);
			color = v3f_apply_gamma(color, GAMMA);
			mlx_put_pixel(img, x, y, v3f_to_col32(color));
			++x;
		}
		++y;
	}
	debug_scene_setup(scene);
}
