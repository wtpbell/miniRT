/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   render.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/10 17:15:02 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/29 19:00:03 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h> 

#include "MLX42/MLX42.h"
#include "libft.h"
#include "minirt.h"
#include "scene.h"
#include "color.h"
#include "rt_math.h"
#include "container.h"
#include "color.h"

#include "debug/rt_debug.h"

#define MAX_DEPTH	5

// PCG Random Number Generator
// LCG: state = (state * A + C) % 2^32, [prev state] --×747796405 +2891336453--> [new state]
// permutation step (*state >> 28) top 4, +4  shift between 4 and 19 bits, (*state >> shift_amount) extract high bits
// https://www.pcg-random.org/
// https://www.youtube.com/watch?v=45Oet5qjlms
static inline float	random_float_pcg(uint32_t *state)
{
	uint32_t	result;

	*state = *state * 747796405 + 2891336453; // Linear Congruential Generator (LCG)
	result = ((*state >> ((*state >> 28) + 4)) ^ *state) * 277803737; // permutation step
	result = (result >> 22) ^ result;
	return ((float)result / 4294967295.0f); // normalized to [0, 1]
}

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

t_col32	gradient_color(t_v3f dir, t_col32 b)
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
		if (obj->intersect(obj, ray, init_v2f(BIAS, *t), &dst))
		{
			// printf("Obj type: %d, intersect: %p\n", obj->type, (void*)obj->intersect);
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

static	float	apply_specular(t_ray_hit *hit, t_v3f light_dir, t_v3f c_pos)
{
	t_v3f	view_dir;
	t_v3f	refl_dir;
	float	spec;
	float	specular_strength;

	view_dir = v3f_norm(v3f_sub(c_pos, hit->hit));
	refl_dir = v3f_refl(v3f_scale(light_dir, -1.0f), hit->normal);
	spec = powf(ft_maxf(0.0f, v3f_dot(view_dir, refl_dir)), 
		hit->obj->r.mat->lamb.shininess);
	specular_strength = hit->obj->r.mat->lamb.specular;
	return (spec * specular_strength);
}

static t_col32	apply_point(t_scene *scene, t_ray_hit *hit, t_light *light)
{
	t_ray	ray;
	t_v3f	light_dir;
	t_v3f	light_color;
	float	diffuse;
	float	specular;
	float	shadow_t;

	light_dir = v3f_sub(light->pos, hit->hit);
	ray.origin = v3f_add(hit->hit, v3f_scale(hit->normal, BIAS));
	ray.direction = v3f_norm(light_dir);
	shadow_t = v3f_mag(light_dir);
	if (find_intersection(&ray, scene, &shadow_t) && shadow_t < v3f_mag(light_dir))
		return (init_col32(0, 0, 0, 255));
	diffuse = ft_maxf(0.0f, v3f_dot(hit->normal, ray.direction));
	specular = apply_specular(hit, ray.direction, scene->camera.t.pos);
	light_color = v3f_add(
		v3f_scale(col32_to_v3f(hit->obj->r.col), diffuse),
		v3f_scale((t_v3f){.x = 0.5f, .y = 0.5f, .z = 0.5f}, specular)
	);
	return (v3f_to_col32(v3f_scale(light_color, light->intensity)));
}

static t_col32	handle_lambertian(t_scene *scene, t_ray_hit *hit_info)
{
	t_light	*light;
	t_col32	acc_color;
	t_col32	base_color;
	int		i;

	i = 0;
	acc_color = init_col32(0, 0, 0, 255);
	base_color = v3f_to_col32(hit_info->obj->r.mat->albedo);
	while (i < scene->lights.size)
	{
		light = (t_light *)scene->lights.items[i];
		if (light->type == LIGHT_AMBIENT)
			acc_color = add_colors_clamped(acc_color, apply_ambient(base_color, light));
		else if (light->type == LIGHT_POINT)
			acc_color = add_colors_clamped(acc_color, apply_point(scene, hit_info, light));
		++i;
	}
	return (apply_gamma(acc_color, GAMMA));
}

//magic numbers (0x9e3779b9 and 0x6d0f27bd) are large primes
static t_col32	handle_dielectric(t_scene *scene, t_ray_hit *hit_info,uint32_t depth)
{
	t_ray		scatter;
	float		refract_ratio;
	float		cos_theta;
	float		sin_theta;
	t_v3f		unit_dir;
	t_col32		out_color;
	uint32_t	state;

	state = scene->frame_num * 0x9e3779b9 + depth * 0x6d0f27bd;
	unit_dir = v3f_unit(hit_info->ray->direction);
	if (hit_info->front_face && hit_info->obj->r.mat->diel.ir!= 0.0f)
		refract_ratio = 1.0f / hit_info->obj->r.mat->diel.ir;
	else
		refract_ratio = hit_info->obj->r.mat->diel.ir;
	cos_theta = fminf(1.0f, v3f_dot(v3f_scale(unit_dir, -1.0f), hit_info->normal));
	sin_theta = sqrtf(1.0f - cos_theta * cos_theta);
	scatter.origin = hit_info->hit;
	if (refract_ratio * sin_theta > 1.0f
		|| v3f_schlick(cos_theta, refract_ratio) > random_float_pcg(&state))
		scatter.direction = v3f_refl(unit_dir, hit_info->normal);
	else
		scatter.direction = v3f_refr(unit_dir, hit_info->normal, refract_ratio);
	out_color = col32_scale(trace(&scatter, scene, depth - 1), hit_info->obj->r.mat->diel.transmittance);
	return (out_color);
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

t_col32	trace(t_ray *ray, t_scene *scene, uint32_t depth)
{
	float		t;
	t_obj		*hit;
	t_ray_hit	hit_info;
	t_col32		color;

	if (depth <= 0)
		return init_col32(0, 0, 0, 255);

	hit = find_intersection(ray, scene, &t);
	if (hit == NULL)
	{
		return (init_col32(255, 255, 255, 255));//gradient_color(ray->direction, scene->camera.bg_col);

	}
	init_hit_info(&hit_info, hit, ray, t);
	hit_info.ray = ray;

	if (hit->r.mat->type == MAT_LAMBERTIAN)
		color = handle_lambertian(scene, &hit_info);
	else if (hit->r.mat->type == MAT_DIELECTRIC)
		color = handle_dielectric(scene, &hit_info, depth);
	// else if (hit->r.mat.type == MAT_METAL)
	// 	color = handle_metal(scene, &hit_info);
	else
		color = init_col32(255, 0, 255, 255);
	return (color);
}

static void	compute_ray(uint32_t x, uint32_t y, t_cam *cam, t_ray *ray)
{
	t_v3f	camera_space;
	float	view;

	view = tanf(cam->fov * 0.5f * DEGTORAD);
	camera_space.x = (2.0f * ((x + 0.5f) / cam->img_plane->width) - 1.0f) * cam->aspect_ratio * view;
	camera_space.y = (1.0f - 2.0f * ((y + 0.5f) / cam->img_plane->height)) * view;
	camera_space.z = 1.0f;
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

static t_col32	anti_aliasing(t_scene *scene, t_ray *ray, uint32_t x, uint32_t y)
{
	uint32_t	s;
	uint32_t	r;
	uint32_t	g;
	uint32_t	b;
	uint32_t	state;

	r = g = b = 0;
	s = 0;
	while (s < SAMPLES_PER_PIXEL)
	{
		state = get_rngstate(x, y, s + scene->frame_num);
		compute_ray((float)x + random_float_pcg(&state), (float)y + random_float_pcg(&state), &scene->camera, ray);
		t_col32 sample = trace(ray, scene, MAX_DEPTH);
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
	static bool first_frame = true;

	img = scene->camera.img_plane;
	invert_m4x4(inv, scene->camera.view_matrix);
	ray.origin = mul_v3_m4x4(init_v3f(0, 0, 0), inv);
	if (first_frame)
	{
		scene->frame_num = 0;
		first_frame = false;
	}
	else
		scene->frame_num++;
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

