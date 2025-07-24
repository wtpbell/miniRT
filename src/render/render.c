/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 17:15:02 by jboon             #+#    #+#             */
/*   Updated: 2025/07/25 00:25:42 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>

#include "MLX42/MLX42.h"
#include "light.h"
#include "material.h"
#include "random.h"
#include "ray.h"
#include "rt_math.h"
#include "scene.h"
#include <stdio.h>

#define MAX_DEPTH			8
#define SAMPLES_PER_PIXEL	8

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
	hit_info->ray = ray;
	hit_info->hit = v3f_add(ray->origin, v3f_scale(ray->direction, t));
	hit_info->normal = obj->calc_norm(obj, hit_info->hit);
	hit_info->distance = t;
	hit_info->front_face = v3f_dot(ray->direction, hit_info->normal) < 0;
	hit_info->obj = obj;
	if (!hit_info->front_face)
		hit_info->normal = v3f_scale(hit_info->normal, -1.0f);
	hit_info->texcoord = obj->r.get_texcoord(obj, hit_info->hit);
	hit_info->texcoord = v2f_rotate(hit_info->texcoord,
			obj->r.mat->texture.scale_rot.theta * DEGTORAD);
	if (obj->r.mat && obj->r.mat->bump_map)
		hit_info->normal = perturb_normal(obj->r.mat, hit_info->texcoord,
				hit_info->normal);
	hit_info->hit_color = obj->r.mat->get_texcol(&hit_info->texcoord,
			&obj->r.mat->texture, v3f_mul(obj->r.color, obj->r.mat->albedo));
}

t_v3f	trace(t_ray *ray, t_scene *scene, uint32_t depth)
{
	float		t;
	t_obj		*hit;
	t_ray_hit	hit_info;
	t_v3f		color;

	if (depth <= 0)
		return (g_v3f_zero);
	hit = find_intersection(ray, scene, &t);
	if (hit == NULL)
		return (g_v3f_zero);
	init_hit_info(&hit_info, hit, ray, t);
	if (hit->r.mat->type == MAT_LAMBERTIAN)
		color = handle_lambertian(scene, &hit_info);
	else if (hit->r.mat->type == MAT_DIELECTRIC)
		color = handle_dielectric(scene, &hit_info, depth);
	else if (hit->r.mat->type == MAT_METAL)
		color = handle_metal(scene, &hit_info, depth);
	else if (hit->r.mat->type == MAT_NORMAL)
		color = display_normal(&hit_info);
	else
		color = (g_v3f_one);
	return (color);
}

static t_v3f	sample_pixel(t_scene *scene, float x, float y)
{
	t_v3f	color;
	t_ray	ray;
	t_v2f	uv;
	t_v2f	jitter;
	int		i;

	color = g_v3f_zero;
	i = 0;
	while (i < SAMPLES_PER_PIXEL)
	{
		seed_rand(get_rngstate(x, y, i));
		jitter.x = frandom_norm_distribution() - 0.5f;
		jitter.y = frandom_norm_distribution() - 0.5f;
		uv.u = (x + 0.5f + jitter.x)
			/ (float)(scene->camera.img_plane->width - 1);
		uv.v = 1.0f - (y + 0.5f + jitter.y)
			/ (float)(scene->camera.img_plane->height - 1);
		ray = get_ray_with_dof(&scene->camera, uv.u, uv.v);
		color = v3f_add(color, trace(&ray, scene, MAX_DEPTH));
		++i;
	}
	return (v3f_scale(color, 1.0f / (float)SAMPLES_PER_PIXEL));
}

void	render(t_scene *scene)
{
	uint32_t	x;
	uint32_t	y;
	t_v3f		color;
	mlx_image_t	*img;

	img = scene->camera.img_plane;
	update_camera_view(&scene->camera);
	y = 0;
	while (y < img->height)
	{
		x = 0;
		while (x < img->width)
		{
			color = sample_pixel(scene, (float)x, (float)y);
			color = v3f_apply_gamma(color, GAMMA);
			mlx_put_pixel(img, x, y, v3f_to_col32(color));
			++x;
		}
		++y;
	}
	debug_scene_setup(scene);
}

void	render_scene(t_game *game)
{
	static bool scene_initialized = false;
	if (!scene_initialized) {
		printf("render_scene: Initial scene render...\n");
		scene_initialized = true;
	}
	update_camera_view(&game->scene->camera);
	render(game->scene);
	if (game->scene->camera.img_plane) {
		static bool image_added = false;
		if (!image_added && game->scene->camera.img_plane->instances)
		{
			mlx_set_instance_depth(game->scene->camera.img_plane->instances, 0);
			image_added = true;
		}
	}
	if (game->ui)
	{
		if (game->ui->needs_redraw)
			game->ui->needs_redraw = false;
	}
	printf("render_scene: Done\n");
}
