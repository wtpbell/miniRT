/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 17:15:02 by jboon             #+#    #+#             */
/*   Updated: 2025/05/26 21:48:11 by bewong           ###   ########.fr       */
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
	obj_to_world(obj->t.to_world, obj->t.pos, obj->t.dir, up);
	invert_m4x4(obj->t.to_obj, obj->t.to_world);
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

static	t_col32	apply_ambient(t_col32 base_col, t_light *light)
{
	return (init_col32(
		get_r(base_col) * light->intensity,
		get_g(base_col) * light->intensity,
		get_b(base_col) * light->intensity,
		255
	));
}

static int	ft_min(int a, int b)
{
	if (a < b)
		return (a);
	return (b);
}

static t_col32	apply_point(t_scene *scene, t_col32 base_col, t_light *light, t_ray *shadow_ray)
{
	t_v3f	light_dir;
	float	light_distance;
	float	t;
	float	diffuse;
	t_ray	offset_ray;
	
	light_dir = v3f_sub(light->pos, shadow_ray->origin);
	light_distance = v3f_dist(light->pos, shadow_ray->origin);
	light_dir = v3f_scale(light_dir, 1.0f/light_distance);
	offset_ray.origin = v3f_add(shadow_ray->origin, v3f_scale(shadow_ray->direction, BIAS));
	offset_ray.direction = light_dir;
	if (find_intersection(&offset_ray, scene, &t) && t > 0.0f && t < light_distance)
		return (init_col32(0, 0, 0, 255));
		
	diffuse = ft_maxf(0.0f, v3f_dot(shadow_ray->direction, light_dir));
	return (init_col32(
		get_r(base_col) * diffuse * light->intensity,
		get_g(base_col) * diffuse * light->intensity,
		get_b(base_col) * diffuse * light->intensity,
		255
	));
}

static t_col32 add_colors(t_col32 a, t_col32 b)
{
	return (init_col32(
		ft_min(255, get_r(a) + get_r(b)),
		ft_min(255, get_g(a) + get_g(b)),
		ft_min(255, get_b(a) + get_b(b)),
		255
	));
}

static t_col32	trace(t_ray *ray, t_scene *scene, uint32_t depth)
{
	float	t;
	t_obj	*hit;
	t_ray	shadow_ray;
	bool	front_face;
	t_col32	accumulated_light;
	int		i;

	(void)depth;
	hit = find_intersection(ray, scene, &t);
	if (hit == NULL)
		return (gradient_color(ray->direction, scene->camera.bg_col));
	shadow_ray.origin = v3f_add(ray->origin, v3f_scale(ray->direction, t));
	shadow_ray.direction = hit->calc_norm(hit, shadow_ray.origin);
	front_face = (v3f_dot(ray->direction, shadow_ray.direction) < 0);
	if (!front_face)
		shadow_ray.direction = v3f_scale(shadow_ray.direction, -1.0f);
	i = 0;
	accumulated_light = init_col32(0, 0, 0, 255); 
	while (i < scene->lights.size)
	{
		t_light *light = (t_light *)scene->lights.items[i];
		if (light->type == LIGHT_AMBIENT)
			accumulated_light = add_colors(accumulated_light, 
				apply_ambient(hit->r.col, light));
		else if (light->type == LIGHT_POINT)
			accumulated_light = add_colors(accumulated_light,
				apply_point(scene, hit->r.col, light, &shadow_ray));
		i++;
	}
	return (add_colors(init_col32(0, 0, 0, 255), accumulated_light));
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

// LCG: state = (state * A + C) % 2^32, [prev state] --×747796405 +2891336453--> [new state]
// permutation step (*state >> 28) top 4, +4  shift between 4 and 19 bits, (*state >> shift_amount) extract high bits
// https://www.pcg-random.org/
// https://www.youtube.com/watch?v=45Oet5qjlms
inline float	random_float_pcg(uint32_t *state)
{
	uint32_t	result;

	*state = *state * 747796405 + 2891336453; // Linear Congruential Generator (LCG)
	result = ((*state >> ((*state >> 28) + 4)) ^ *state) * 277803737; // permutation step
	result = (result >> 22) ^ result;
	return ((float)result / 4294967295.0f); // normalized to [0, 1]
}

#define SAMPLES_PER_PIXEL 1

static t_col32	anti_aliasing(t_scene *scene, t_ray *ray, uint32_t x, uint32_t y)
{
	uint32_t	s;
	uint32_t	r;
	uint32_t	g;
	uint32_t	b;
	uint32_t	state;

	r = g = b = 0;
	state = (uint32_t)time(NULL);
	s = 0;
	while (s < SAMPLES_PER_PIXEL)
	{
		compute_ray((float)x + (random_float_pcg(&state)), (float)y + (random_float_pcg(&state)), &scene->camera, ray);
		t_col32 sample = trace(ray, scene, 0);
		r += get_r(sample);
		g += get_g(sample);
		b += get_b(sample);
		++s;
	}
	return (init_col32(
		r / SAMPLES_PER_PIXEL,
		g / SAMPLES_PER_PIXEL,
		b / SAMPLES_PER_PIXEL,
		255));
}

void	render(t_scene *scene)
{
	uint32_t	x;
	uint32_t	y;
	t_ray		ray;
	mlx_image_t	*img;
	t_mat4x4	inv;

	img = scene->camera.img_plane;
	invert_m4x4(inv, scene->camera.view_matrix);
	ray.origin = mul_v3_m4x4(init_v3f(0, 0, 0), inv);

	y = 0;
	while (y < img->height)
	{
		x = 0;
		while (x < img->width)
		{
			mlx_put_pixel(img, x, y, anti_aliasing(scene, &ray, x, y));
			++x;
		}
		++y;
	}
}

