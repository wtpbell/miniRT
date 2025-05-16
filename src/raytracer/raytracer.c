/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   raytracer.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/10 17:15:02 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/16 14:56:29 by jboon         ########   odam.nl         */
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

bool	geo_intersect(t_sphere *sp, t_ray *ray, t_ray_hit *hitinfo)
{
	t_v3f	L;
	float	tca;
	float	d;
	float	thc;
	float	t0;
	float	t1;

	L = v3f_sub(sp->t.pos, ray->origin);
	tca = v3f_dot(L, ray->direction);
	if (tca < 0)
		return (false);
	d = v3f_dot(L, L) - tca * tca;
	if (d > sp->radius * sp->radius)
		return (false);
	thc = sqrtf(sp->radius * sp->radius - d);
	t0 = tca - thc;
	t1 = tca + thc;
	if (t0 < t1)
		ft_swapf(&t0, &t1);
	if (t0 < 0)
	{
		if (t1 < 0)
			return (false);
		t0 = t1;
	}
	hitinfo->hit = v3f_add(ray->origin, v3f_scale(ray->direction, t0));
	hitinfo->distance = v3f_mag(hitinfo->hit);
	return (true);
}

bool	solve_quadratic(t_v3f *abc, float *x0, float *x1)
{
	float	dis;
	float	q;

	dis = abc->y * abc->y - 4 * abc->x * abc->z;
	if (dis < 0)
		return (false);
	else if (dis == 0)
	{
		*x0 = -0.5f * abc->y / abc->x;
		*x1 = *x0;
	}
	else
	{
		q = -0.5f * (abc->y + ft_signf(abc->y) * sqrtf(dis));
		*x0 = q / abc->x;
		*x1 = abc->z / q;
	}
	if (*x0 > *x1)
		ft_swapf(x0, x1);
	return (true);
}

bool	intersect(t_sphere *sp, t_ray *ray, float *dst)
{
	t_v3f	L;
	t_v3f	abc;
	float	t0;
	float	t1;

	L = v3f_sub(ray->origin, sp->t.pos);
	abc.x = v3f_dot(ray->direction, ray->direction);
	abc.y = 2.0f * v3f_dot(ray->direction, L);
	abc.z = v3f_dot(L, L) - sp->radius * sp->radius;
	if (!solve_quadratic(&abc, &t0, &t1))
		return (false);
	if (t0 < 0)
	{
		if (t1 < 0)
			return (false);
		t0 = t1;
	}
	*dst = t0;
	return (true);
}

t_col32	trace(t_ray *ray, t_scene *scene, uint32_t depth)
{
	int			i;
	float		min_dist;
	float		curr_dst;
	t_sphere	*hit;
	// t_ray_hit	curr_hit;
	t_vector	objects;

	(void)depth;
	i = 0;
	hit = NULL;
	min_dist = FLT_MAX;
	objects = scene->objects;
	while (i < objects.size)
	{
		if (intersect(objects.items[i], ray, &curr_dst)
			&& curr_dst < min_dist)
			{
				printf(".");
				min_dist = curr_dst;
				hit = objects.items[i];
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
		// curr_hit.hit = v3f_add(ray->origin, v3f_scale(ray->direction, min_dist));
		// curr_hit.normal = v3f_norm(v3f_sub(curr_hit.hit, hit->t.pos));
		// curr_hit.normal = v3f_scale(v3f_add(curr_hit.normal, init_v3f(1, 1, 1)), 255>>1);
		// return (init_col32(curr_hit.normal.x, curr_hit.normal.y, curr_hit.normal.z, 255));
		return (hit->r.col);
	}
	return (scene->camera.bg_col);
}

void	compute_ray(uint32_t x, uint32_t y, t_camera *cam, t_ray *ray)
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

	printf("%i items\n", scene->objects.size);
	while (y < img->height)
	{
		x = 0;
		while (x < img->width)
		{
			compute_ray(x, y, &scene->camera, &ray);
			ray.direction = v3f_norm(mul_dir_m4x4(ray.direction, scene->camera.cam_to_world));
			mlx_put_pixel(img, x, y, trace(&ray, scene, 0));
			++x;
		}
		++y;
	}
	printf("done!\n");
}
