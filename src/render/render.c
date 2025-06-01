/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 17:15:02 by jboon             #+#    #+#             */
/*   Updated: 2025/06/01 19:04:43 by bewong           ###   ########.fr       */
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

static t_v3f	handle_lambertian(t_scene *scene, t_ray_hit *hit_info)
{
	t_light	*light;
	t_v3f	total_light;
	t_v3f	obj_albedo;
	int		i;

	total_light = (t_v3f){{0.0f, 0.0f, 0.0f}};
	obj_albedo = hit_info->obj->r.mat->albedo;
	i = 0;
	while (i < scene->lights.size)
	{
		light = (t_light *)scene->lights.items[i];
		if (light->type == LIGHT_POINT)
			total_light = v3f_add(total_light, apply_point(scene, hit_info, light));
		else if (light->type == LIGHT_AMBIENT)
			total_light = v3f_add(total_light, apply_ambient(obj_albedo, light));
		i++;
	}
	return ((t_v3f){{
		.x = obj_albedo.x * total_light.x,
		.y = obj_albedo.y * total_light.y,
		.z = obj_albedo.z * total_light.z
	}});
}

static float	get_refraction_ratio(t_ray_hit *hit_info)
{
	if (hit_info->front_face)
		return (1.0f / hit_info->obj->r.mat->diel.ir);
	return (hit_info->obj->r.mat->diel.ir);
}

static t_v3f	blend_color(t_scene *sc, t_ray_hit *hit, uint32_t depth, float ior)
{
	t_ray	ray;
	t_v3f	colors[2];
	float	cos_theta;
	float	reflectance;

	ray.direction = v3f_norm(hit->ray->direction);
	if (hit->front_face && hit->obj->r.mat->diel.ir != 0.0f)
		ior = 1.0f / hit->obj->r.mat->diel.ir;
	else
		ior = hit->obj->r.mat->diel.ir;
	cos_theta = fminf(1.0f, v3f_dot(v3f_scale(ray.direction, -1.0f), hit->normal));
	reflectance = schlick(cos_theta, ior);
	if (hit->front_face)
		ray.origin = v3f_add(hit->hit, v3f_scale(hit->normal, 0.001f));
	else
		ray.origin = v3f_add(hit->hit, v3f_scale(hit->normal, -0.001f));
	ray.direction = v3f_refl(ray.direction, hit->normal);
	colors[0] = trace(&ray, sc, depth - 1);
	if (ior * sqrtf(1.0f - cos_theta * cos_theta) <= 1.0f)
	{
		ray.direction = v3f_refr(ray.direction, hit->normal, ior);
		colors[1] = trace(&ray, sc, depth - 1);
	}
	else
		colors[1] = (t_v3f){{0.0f, 0.0f, 0.0f}};
	return (v3f_lerp(colors[1], colors[0], reflectance));
}

// 0.1f more perfect, 0.3f more visible diffuse lighting, 0.0f pure transparent
static t_v3f	handle_dielectric(t_scene *sc, t_ray_hit *hit, uint32_t depth)
{
	t_v3f	direct;
	t_v3f	indirect;
	t_v3f	final;
	float	kd;

	if (depth == 0)
		return ((t_v3f){{0.0f, 0.0f, 0.0f}});
	direct = handle_lambertian(sc, hit);
	indirect = blend_color(sc, hit, depth, get_refraction_ratio(hit));
	kd = 1.0f - hit->obj->r.mat->diel.transmittance - 0.2f;  // Diffuse weight = 1.0 - transmittance - 0.2f(small bias)
	final = (t_v3f){{
		.x = (direct.x * kd + indirect.x * (1.0f - kd)) * hit->obj->r.mat->albedo.x,
		.y = (direct.y * kd + indirect.y * (1.0f - kd)) * hit->obj->r.mat->albedo.y,
		.z = (direct.z * kd + indirect.z * (1.0f - kd)) * hit->obj->r.mat->albedo.z
	}};
	return (final);
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
	float	t;
	t_obj		*hit;
	t_ray_hit	hit_info;
	t_v3f		color;

	if (depth <= 0)
		return (t_v3f){{0.0f, 0.0f, 0.0f}};
	hit = find_intersection(ray, scene, &t);
	if (hit == NULL)
		return (t_v3f){{1.0f, 1.0f, 1.0f}};
	init_hit_info(&hit_info, hit, ray, t);
	hit_info.ray = ray;
	if (hit->r.mat->type == MAT_LAMBERTIAN)
		color = handle_lambertian(scene, &hit_info);
	else if (hit->r.mat->type == MAT_DIELECTRIC)
		color = handle_dielectric(scene, &hit_info, depth);
	// else if (hit->r.mat->type == MAT_METAL)
	// 	color = handle_metal(scene, &hit_info, depth);
	else
		color = (t_v3f){{1.0f, 0.0f, 1.0f}};
	return (v3f_clamp(color, 0.0f, 1.0f));
}

static void	compute_ray(uint32_t x, uint32_t y, t_cam *cam, t_ray *ray)
{
	t_v3f	camera_space;
	float	view;

	view = tanf(cam->fov * 0.5f * DEGTORAD);
	camera_space.x = (2.0f * ((x + 0.5f) / cam->img_plane->width) - 1.0f) * cam->aspect_ratio * view;
	camera_space.y = (1.0f - 2.0f * ((y + 0.5f) / cam->img_plane->height)) * view;
	camera_space.z = -1.0f;
	ray->direction = v3f_norm(mul_dir_m4x4(camera_space, cam->view_matrix));
}


#define SAMPLES_PER_PIXEL 1

// x * large prime number in hex, ^ y to mix x and y
// + frame * large prime number in hex to mix frame
// 0x9e3779b9 golden ratio prime
uint32_t	get_rngstate(uint32_t x, uint32_t y, uint32_t frame)
{
	uint32_t	hash;

	hash = (x * 0x1f1f1f1f) ^ y;
	return (hash + frame * 0x9e3779b9);
}

static t_v3f	anti_aliasing(t_scene *scene, t_ray *ray, uint32_t x, uint32_t y)
{
	t_v3f		color;
	t_v3f		final_color;
	uint32_t	s;
	uint32_t	rng_state;

	final_color = (t_v3f){{0.0f, 0.0f, 0.0f}};
	s = 0;
	while (s < SAMPLES_PER_PIXEL)
	{
		rng_state = get_rngstate(x, y, s + scene->frame_num);
		compute_ray((float)x + random_float_pcg(&rng_state),
			(float)y + random_float_pcg(&rng_state), &scene->camera, ray);
		color = trace(ray, scene, MAX_DEPTH);
		final_color = v3f_add(final_color, color);
		s++;
	}
	return (v3f_scale(final_color, 1.0f / SAMPLES_PER_PIXEL));
}

void	render(t_scene *scene)
{
	uint32_t	x;
	uint32_t	y;
	t_ray		ray;
	mlx_image_t	*img;
	t_mat4x4	inv;
	t_v3f		color;

	debug_scene_setup(scene);
	img = scene->camera.img_plane;
	invert_m4x4(inv, scene->camera.view_matrix);
	ray.origin = mul_v3_m4x4(init_v3f(0, 0, 0), inv);
	y = 0;
	while (y < img->height)
	{
		x = 0;
		while (x < img->width)
		{
			color = anti_aliasing(scene, &ray, x, y);
			color = v3f_apply_gamma(color, GAMMA);
			mlx_put_pixel(img, x, y, v3f_to_col32(color));
			++x;
		}
		++y;
	}
	debug_scene_setup(scene);
}
