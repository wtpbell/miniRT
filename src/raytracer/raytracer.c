/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   raytracer.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/10 17:15:02 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/15 15:28:58 by jboon         ########   odam.nl         */
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
		*x0 = -0.5f * abc->y * abc->x;
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
	*dst = v3f_mag(v3f_add(ray->origin, v3f_scale(ray->direction, t0)));
	return (true);
}

// https://stackoverflow.com/questions/18558910/direction-vector-to-rotation-matrix (column major, right-handed)
void	cam_to_world_mat(t_mat4x4 mat, t_v3f pos, t_v3f dir, t_v3f up)
{
	t_v3f	x_axis;
	t_v3f	y_axis;

	id_m4x4(mat);
	trans_m4x4(mat, pos);
	x_axis = v3f_norm(v3f_cross(up, dir));
	y_axis = v3f_norm(v3f_cross(dir, x_axis));
	mat[0] = -x_axis.x;
	mat[1] = x_axis.y;
	mat[2] = x_axis.z;
	mat[4] = y_axis.x;
	mat[5] = y_axis.y;
	mat[6] = y_axis.z;
	mat[8] = dir.x;
	mat[9] = dir.y;
	mat[10] = dir.z;
}

bool	t = true;

t_col32	trace(t_ray *ray, t_vector *objects, t_light *light, t_col32 bg_col, uint32_t depth)
{
	int			i;
	t_ray		shadow_ray;
	float		min_dist;
	float		curr_dst;
	t_sphere	*hit;
	t_ray_hit	curr_hit;

	(void)depth;
	i = 0;
	hit = NULL;
	min_dist = FLT_MAX;
	while (i < objects->size)
	{
		if (intersect(objects->items[i], ray, &curr_dst)
			&& curr_dst < min_dist)
			{
				min_dist = curr_dst;
				hit = objects->items[i];
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

		curr_hit.normal = v3f_norm(v3f_sub(curr_hit.hit, hit->t.pos));
		curr_hit.normal = v3f_scale(v3f_add(curr_hit.normal, init_v3f(1, 1, 1)), 255>>1);

		return (init_col32(curr_hit.normal.x, curr_hit.normal.y, curr_hit.normal.z, 255));
	}
	// IF GLASS && DEPTH < MAX_DEPTH
	// Calc reflection, refraction
	// Call trace(refraction_ray, depth + 1)
	// Calc fresnel effect
	// Return color
	// ELSE
	// Calc shadow_ray
	// END IF
	i = 0;
	shadow_ray.origin = v3f_add(ray->origin, v3f_scale(ray->direction, min_dist));
	shadow_ray.direction = v3f_sub(light->pos, ray->origin);
	while (i < objects->size)
	{
		if (intersect(objects->items[i], &shadow_ray, NULL))
			return (((t_sphere *)(objects->items + i))->r.col * light->intensity);
		++i;
	}
	return (bg_col);
}

void	create_objects(t_vector *objects)
{
	t_sphere	*ptr_sp;
	t_sphere	sp;

	ft_bzero(&sp, sizeof(t_sphere));

	vector_init(objects, 4);

	sp.t.pos = init_v3f(0, 0, 0);
	sp.r.col = C_RED;
	sp.radius = 3;
	ptr_sp = malloc(sizeof(t_sphere));
	*ptr_sp = sp;
	vector_add(objects, ptr_sp);

	sp.t.pos = init_v3f(6, 0, 0);
	sp.r.col = C_GREEN;
	sp.radius = 2;
	ptr_sp = malloc(sizeof(t_sphere));
	*ptr_sp = sp;
	vector_add(objects, ptr_sp);

	sp.t.pos = init_v3f(-5, 0, 0);
	sp.r.col = C_BLUE;
	sp.radius = 1;
	ptr_sp = malloc(sizeof(t_sphere));
	*ptr_sp = sp;
	vector_add(objects, ptr_sp);

	sp.t.pos = init_v3f(0, 0, 30);
	sp.r.col = C_BLUE | C_RED;
	sp.radius = 10;
	ptr_sp = malloc(sizeof(t_sphere));
	*ptr_sp = sp;
	vector_add(objects, ptr_sp);
}

// TODO: Take the position and direction of the camera into account
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

	ray->direction = v3f_norm(camera_space); // This assumes the ray origin at (0, 0, 0)
}

void	render(mlx_image_t *img, t_col32 bg_col)
{
	uint32_t	x;
	uint32_t	y;
	t_ray		ray;
	t_vector	objects;
	t_light		light;
	t_camera	cam;
	t_mat4x4	mat;

	cam = (t_camera){
		.t = {
			.pos = {.x = 0, .y = 0, .z = -10},
			.dir = {.x = 0, .y = 0, .z = -1}
		},
		.fov = 70,
		.img_plane = img,
		.aspect_ratio = img->width / (float)img->height
	};

	cam.t.dir = v3f_norm(cam.t.dir);
	cam_to_world_mat(mat, cam.t.pos, cam.t.dir, init_v3f(0, 1, 0));
	v3f_print(cam.t.dir);
	mat4x4_print(mat);
	mat4x4_rot_print(mat);

	y = 0;
	light.intensity = .75f;
	light.pos = init_v3f(0, 30, 5);
	ray.origin = mul_v3_m4x4(init_v3f(0, 0, 0), mat);
	v3f_print(ray.origin);
	create_objects(&objects);
	while (y < img->height)
	{
		x = 0;
		while (x < img->width)
		{
			compute_ray(x, y, &cam, &ray);

			ray.direction = v3f_norm(mul_dir_m4x4(ray.direction, mat));

			mlx_put_pixel(img, x, y, trace(&ray, &objects, &light, bg_col, 0));
			++x;
		}
		++y;
	}
	vector_free(&objects, free);
}
