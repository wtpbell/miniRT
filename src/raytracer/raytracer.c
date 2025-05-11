/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   raytracer.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/10 17:15:02 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/11 21:29:21 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


#include <stdint.h>
#include <stdlib.h>

#include "MLX42/MLX42.h"
#include "libft.h"
#include "scene.h"
#include "color.h"
#include "rt_math.h"
#include "container.h"

typedef struct s_ray
{
	t_v3f	origin;
	t_v3f	direction;
}	t_ray;

typedef struct s_ray_hit
{
	t_v3f		hit;
	t_v3f		normal;
	float		distance;
	t_sphere	*sphere;
}	t_ray_hit;

void	compute_prime_ray(uint32_t x, uint32_t y, t_ray *ray);
bool	intersect(t_sphere *sp, t_ray *ray, t_ray_hit *result);

inline t_ray_hit	init_ray_hit(void)
{
	return ((t_ray_hit){
		.hit = {0},
		.normal = {0},
		.distance = FLT_MAX,
		.sphere = NULL
	});
}

t_col32	trace(t_ray *ray, t_vector *objects, t_light *light, t_col32 bg_col, uint32_t depth)
{
	uint32_t	i;
	t_ray		shadow_ray;
	t_ray_hit	closest_hit;
	t_ray_hit	curr_hit;

	(void)depth;
	i = 0;
	closest_hit = init_ray_hit();
	while (i < objects->size)
	{
		if (intersect(objects->items + i, ray, &curr_hit)
			&& curr_hit.distance < closest_hit.distance)
			closest_hit = curr_hit;
	}
	if (closest_hit.sphere == NULL)
		return (bg_col);
	// IF GLASS && DEPTH < MAX_DEPTH
	// Calc reflection, refraction
	// Call trace(refraction_ray, depth + 1)
	// Calc fresnel effect
	// Return color
	// ELSE
	// Calc shadow_ray
	// END IF
	i = 0;
	shadow_ray.origin = v3f_add(closest_hit.hit, closest_hit.normal);
	shadow_ray.direction = v3f_norm(v3f_sub(light->pos, closest_hit.hit));
	while (i < objects->size)
	{
		if (intersect(objects->items + i, &shadow_ray, NULL))
			return (((t_sphere *)(objects->items + i))->r.col * light->intensity);
	}
	return (bg_col);
}

void	create_objects(t_vector *objects)
{
	t_sphere	*ptr_sp;
	t_sphere	sp = (t_sphere){
		.t = {
			.pos = {0},
			.dir = {0}
		},
		.r = {
			.mat = {0},
			.col = {0}
		},
		.radius = 50
	};

	vector_init(objects, 3);

	sp.t.pos = init_v3f(0, 0, 20);
	sp.r.col = C_RED;
	ptr_sp = malloc(sizeof(t_sphere));
	*ptr_sp = sp;
	vector_add(objects, ptr_sp);

	sp.t.pos = init_v3f(5, 0, 5);
	sp.r.col = C_GREEN;
	sp.radius = 15;
	ptr_sp = malloc(sizeof(t_sphere));
	*ptr_sp = sp;
	vector_add(objects, ptr_sp);

	sp.t.pos = init_v3f(-5, 0, 10);
	sp.r.col = C_BLUE;
	sp.radius = 25;
	ptr_sp = malloc(sizeof(t_sphere));
	*ptr_sp = sp;
	vector_add(objects, ptr_sp);
}

void	render(mlx_image_t *img, t_col32 bg_col)
{
	uint32_t	x;
	uint32_t	y;
	t_ray		ray;
	t_vector	objects;
	t_light		light;

	y = 0;
	light.intensity = .75f;
	light.pos = init_v3f(0, 30, 5);
	create_objects(&objects);
	while (y < img->height)
	{
		x = 0;
		while (x < img->width)
		{
			compute_prime_ray(x, y, &ray);
			mlx_put_pixel(img, x, y, trace(&ray, &objects, &light, bg_col, 0));
		}
	}
	vector_free(&objects, free);
}
