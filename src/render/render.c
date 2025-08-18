/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   render.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/10 17:15:02 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/18 15:04:32 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "MLX42/MLX42.h"
#include "light.h"
#include "material.h"
#include "minirt.h"
#include "random.h"
#include "ray.h"
#include "rt_math.h"
#include "scene.h"
#include "ui.h"
#include "rt_thread.h"

t_obj	*find_intersection(t_ray *ray, t_scene *scene, t_result *res)
{
	int			i;
	t_result	curr;
	t_obj		*obj;
	t_obj		*hit;

	i = 0;
	hit = NULL;
	res->t = FLT_MAX;
	ft_bzero(&curr, sizeof(ft_bzero));
	while (i < scene->objects.size)
	{
		obj = (t_obj *)scene->objects.items[i];
		if (obj->intersect(obj, ray, init_v2f(BIAS, res->t), &curr))
		{
			*res = curr;
			hit = obj;
		}
		++i;
	}
	return (hit);
}

static void	init_hit_info(t_ray_hit *hit_info, t_obj *obj, t_ray *ray,
	t_result *res)
{
	hit_info->ray = ray;
	hit_info->hit = v3f_add(ray->origin, v3f_scale(ray->direction, res->t));
	hit_info->normal = obj->calc_norm(obj, hit_info->hit, res);
	hit_info->distance = res->t;
	hit_info->front_face = v3f_dot(ray->direction, hit_info->normal) < 0;
	hit_info->obj = obj;
	hit_info->weight = res->tri_weight;
	if (!hit_info->front_face)
		hit_info->normal = v3f_scale(hit_info->normal, -1.0f);
	hit_info->texcoord = obj->r.get_texcoord(obj, hit_info->hit, res);
	hit_info->texcoord = v2f_rotate(hit_info->texcoord,
			obj->r.mat->texture.scale_rot.w * DEGTORAD);
	if (obj->r.mat->bump_map.type != TEX_SOLID)
		hit_info->normal = perturb_normal(obj->r.mat, hit_info->texcoord,
				hit_info->normal);
	hit_info->hit_color = obj->r.mat->get_texcol(&hit_info->texcoord,
			&obj->r.mat->texture, v3f_mul(obj->r.color, obj->r.mat->albedo));
}

t_v3f	trace(t_ray *ray, t_scene *scene, uint32_t depth)
{
	t_obj		*hit;
	t_ray_hit	hit_info;
	t_v3f		color;
	t_result	res;

	if (depth <= 0)
		return (g_v3f_zero);
	ft_bzero(&res, sizeof(res));
	hit = find_intersection(ray, scene, &res);
	if (hit == NULL)
		return (g_v3f_zero);
	init_hit_info(&hit_info, hit, ray, &res);
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

static t_v3f	sample_pixel(t_scene *scene, t_sample *sample, float x, float y)
{
	t_v3f			color;
	t_ray			ray;
	t_v2f			uv;
	t_v2f			jitter;
	uint32_t		i;

	color = g_v3f_zero;
	i = 0;
	while (i < sample->sample_pxl)
	{
		seed_rand(get_rngstate(x, y, i));
		jitter.x = frandom_norm_distribution() - 0.5f;
		jitter.y = frandom_norm_distribution() - 0.5f;
		uv.u = (x + 0.5f + jitter.x)
			/ (float)(scene->camera.img_plane->width - 1);
		uv.v = 1.0f - (y + 0.5f + jitter.y)
			/ (float)(scene->camera.img_plane->height - 1);
		ray = get_ray_with_dof(&scene->camera, uv.u, uv.v);
		color = v3f_add(color, trace(&ray, scene, sample->max_depth));
		++i;
	}
	return (v3f_scale(color, 1.0f / sample->sample_pxl));
}

void	*render(void *ctx)
{
	t_pthread_instr	*instr;
	t_v3f			color;
	uint32_t		y;
	uint32_t		x;

	instr = (t_pthread_instr *)ctx;
	y = instr->start_y;
	while (y < instr->end_y)
	{
		x = 0;
		while (x < instr->img->width)
		{
			color = sample_pixel(instr->shared_data->scene,
					instr->shared_data->sample, (float)x, (float)y);
			color = v3f_apply_gamma(color, GAMMA);
			mlx_put_pixel(instr->img, x, y, v3f_to_col32(color));
			++x;
		}
		++y;
		pthread_mutex_lock(instr->shared_data->progress_lock);
		++(*instr->shared_data->progress_count);
		pthread_mutex_unlock(instr->shared_data->progress_lock);
		pthread_testcancel();
	}
	return (NULL);
}
