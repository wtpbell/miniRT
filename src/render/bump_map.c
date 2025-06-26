/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 09:15:29 by bewong            #+#    #+#             */
/*   Updated: 2025/06/26 15:13:37 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_math.h"
#include "scene.h"
#include "color.h"

// https://learnopengl.com/Advanced-Lighting/Normal-Mapping
// https://learnopengl.com/Advanced-Lighting/Normal-Mapping
// https://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/
//Vec3 normal = Vec3(2*(center_r-center_l), 2*(center_d-center_u), -4).Normalize();

#define DFLT_BUMP_SCALE 0.1f

// sample height of current UV coord, then sample neighbor UV coords to get height difference
static float	sample_bump_map(mlx_texture_t *bump_map, t_v2f uv)
{
	int		x;
	int		y;
	int		idx;
	float	rgb[3];

	uv.x = ft_clampf(uv.x, 0.0f, 1.0f - FLT_SML);
	uv.y = ft_clampf(uv.y, 0.0f, 1.0f - FLT_SML);
	x = (int)(uv.x * (bump_map->width - 1));
	y = (int)(uv.y * (bump_map->height - 1));
	x = x % bump_map->width;
	y = y % bump_map->height;
	if (x < 0)
		x += bump_map->width;
	if (y < 0)
		y += bump_map->height;
	idx = (y * bump_map->width + x) * 4;
	rgb[0] = bump_map->pixels[idx] / 255.0f;
	rgb[1] = bump_map->pixels[idx + 1] / 255.0f;
	rgb[2] = bump_map->pixels[idx + 2] / 255.0f;
	return (0.2126f * rgb[0] + 0.7152f * rgb[1] + 0.0722f * rgb[2]);
}

static t_v3f	apply_bump(t_bump *ctx, t_v2f uv)
{
	t_v2f	uv_u;
	t_v2f	uv_v;
	t_v3f	heights;
	t_v3f	height_deltas;
	float	bump_scale;

	if (!ctx || !ctx->mat || !ctx->mat->bump_map || !ctx->mat->bump_map->pixels)
		return (ctx->n);
	uv_u = (t_v2f){.x = uv.x + ctx->delta, .y = uv.y};
	uv_v = (t_v2f){.x = uv.x, .y = uv.y + ctx->delta};
	heights.x = sample_bump_map(ctx->mat->bump_map, uv);
	heights.y = sample_bump_map(ctx->mat->bump_map, uv_u);
	heights.z = sample_bump_map(ctx->mat->bump_map, uv_v);
	ctx->heights[0] = v3f_scale(g_v3f_one, heights.x);
	ctx->heights[1] = v3f_scale(g_v3f_one, heights.y);
	ctx->heights[2] = v3f_scale(g_v3f_one, heights.z);
	if (ctx->mat->bump_scale > 0.0f)
		bump_scale = ctx->mat->bump_scale;
	else
		bump_scale = DFLT_BUMP_SCALE;
	height_deltas = (t_v3f){
		.x = (heights.y - heights.x) * bump_scale,
		.y = (heights.z - heights.x) * bump_scale,
		.z = 0.0f
	};
	ctx->p = v3f_add(ctx->n, v3f_scale(ctx->t, height_deltas.x));
	ctx->p = v3f_add(ctx->p, v3f_scale(ctx->b, height_deltas.y));
	return (v3f_norm(ctx->p));
}

static void	calc_tangent_basis(t_v3f n, t_v3f *t, t_v3f *b)
{
	t_v3f	up;

	if (fabsf(n.y) < 0.999f)
		up = g_v3f_up;
	else
		up = g_v3f_back;
	*t = v3f_norm(v3f_cross(up, n));
	*b = v3f_norm(v3f_cross(n, *t));
}

t_v3f	perturb_normal(t_obj *obj, t_v3f normal, t_v3f point, const t_mat *mat)
{
	t_bump	ctx;
	t_v2f	uv;

	if (!obj || !obj->uv_map || !mat || !mat->bump_map)
		return (normal);
	uv = obj->uv_map(obj, point);
	ft_bzero(&ctx, sizeof(t_bump));
	ctx.n = normal;
	ctx.mat = mat;
	calc_tangent_basis(normal, &ctx.t, &ctx.b);
	ctx.delta = 1.0f / mat->bump_map->width;
	return (apply_bump(&ctx, uv));
}
