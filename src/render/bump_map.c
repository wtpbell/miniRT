/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   bump_map.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/24 09:15:29 by bewong        #+#    #+#                 */
/*   Updated: 2025/06/24 16:55:23 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "rt_types.h"
#include "scene.h"
#include "material.h"
#include "MLX42/MLX42.h"

//https://www.leadwerks.com/community/topic/16244-calculate-normals-from-heightmap/
//Vec3 normal = Vec3(2*(center_r-center_l), 2*(center_d-center_u), -4).Normalize();

static float	sample_bump_map(mlx_texture_t *bump_map, t_v2f uv)
{
	int		x;
	int		y;
	int		idx;
	float	r;
	float	g;
	float	b;

	x = (int)(uv.x * (bump_map->width - 1)) % bump_map->width;
	y = (int)(uv.y * (bump_map->height - 1)) % bump_map->height;
	idx = (y * bump_map->width + x) * 4;
	r = bump_map->pixels[idx] / 255.0f;
	g = bump_map->pixels[idx + 1] / 255.0f;
	b = bump_map->pixels[idx + 2] / 255.0f;
	return (0.2126f * r + 0.7152f * g + 0.0722f * b);
}

static t_v3f apply_bump(t_v3f n, const t_mat *mat, t_v2f uv, t_v3f t, t_v3f b)
{
	float	h;
	float	h_u;
	float	h_v;
	float	bump;
	float	delta;
	t_v2f	uv_u;
	t_v2f	uv_v;
	t_v3f	p;

	delta = 1.0f / mat->bump_map->width;
	uv_u = init_v2f(uv.x + delta, uv.y);
	uv_v = init_v2f(uv.x, uv.y + delta);
	h = sample_bump_map(mat->bump_map, uv);
	h_u = sample_bump_map(mat->bump_map, uv_u);
	h_v = sample_bump_map(mat->bump_map, uv_v);
	if ((mat->bump_scale > 0.0f))
		bump = mat->bump_scale;
	else
		bump = 0.1f;
	p = v3f_add(n, v3f_scale(t, (h - h_u) * bump));
	p = v3f_add(p, v3f_scale(b, (h - h_v) * bump));
	return (v3f_norm(p));
}

static void calc_tangent_basis(t_v3f n, t_v3f *t, t_v3f *b)
{
	t_v3f up = init_v3f(0, 1, 0);
	t_v3f right = init_v3f(1, 0, 0);

	if (fabsf(n.y) > 0.9f)
		*t = right;
	else
		*t = v3f_norm(v3f_cross(n, up));
	*b = v3f_norm(v3f_cross(n, *t));
}

t_v3f perturb_normal(t_obj *obj, t_v3f normal, t_v3f point, const t_mat *mat)
{
	t_v2f	uv;
	t_v3f	tangent;
	t_v3f	bitangent;

	uv = init_v2f(0.0f, 0.0f);
	uv = obj->uv_map(obj, point);
	calc_tangent_basis(normal, &tangent, &bitangent);
	return (apply_bump(normal, mat, uv, tangent, bitangent));
}


