/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_material_utils.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/02 18:36:09 by jboon         #+#    #+#                 */
/*   Updated: 2025/07/02 19:05:24 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ray.h"
#include "rt_math.h"
#include "minirt.h"
#include "random.h"

static float	get_refraction_ratio(t_ray_hit *hit)
{
	if (hit->front_face && hit->obj->r.mat->diel.ir != 0.0f)
		return (1.0f / hit->obj->r.mat->diel.ir);
	return (hit->obj->r.mat->diel.ir);
}

static inline	t_v3f	get_refl_dir(t_v3f refl, t_v3f normal, float roughness)
{
	return (v3f_norm(
			v3f_add(refl, v3f_scale(random_in_hemisphere(normal), roughness)))
	);
}

static inline bool	is_refr(float ior, t_v3f dir, t_v3f normal)
{
	return (ior
		* sqrtf(1.0 - powf(v3f_dot(v3f_scale(dir, -1.0), normal), 2)) <= 1.0f);
}

static inline t_v3f	get_refr_col(t_ray *ray, t_ray_hit *hitinfo, uint32_t depth,
	t_scene *scene)
{
	ray->direction = v3f_refr(ray->direction, hitinfo->normal,
			get_refraction_ratio(hitinfo));
	return (v3f_mul(trace(ray, scene, depth - 1), hitinfo->hit_color));
}

t_v3f	blend_color(t_scene *sc, t_ray_hit *h, uint32_t depth, float ior)
{
	t_ray	ray;
	t_v3f	refl_dir;
	t_v3f	refl_col;
	t_v3f	refr_col;
	float	fresnel;

	ray.direction = h->ray->direction;
	refl_dir = v3f_scale(ray.direction, -1.0f);
	if (h->front_face)
		ray.origin = v3f_add(h->hit, v3f_scale(h->normal, 0.001f));
	else
		ray.origin = v3f_add(h->hit, v3f_scale(h->normal, -0.001f));
	fresnel = schlick(fminf(1.0f, v3f_dot(refl_dir, h->normal)), ior);
	fresnel = powf(fresnel, 0.5f);
	refl_dir = v3f_refl(ray.direction, h->normal);
	if (h->obj->r.mat->diel.roughness > 0.0f)
		refl_dir = get_refl_dir(refl_dir, h->normal,
				h->obj->r.mat->diel.roughness);
	refl_col = v3f_mul(
			trace(&(t_ray){ray.origin, refl_dir}, sc, depth - 1), h->hit_color);
	refr_col = g_v3f_zero;
	if (is_refr(ior, ray.direction, h->normal))
		refr_col = get_refr_col(&ray, h, depth, sc);
	return (v3f_lerp(refr_col, refl_col, fresnel));
}
