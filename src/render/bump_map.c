/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   bump_map.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/24 09:15:29 by bewong        #+#    #+#                 */
/*   Updated: 2025/07/25 18:42:32 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "rt_math.h"
#include "scene.h"
#include "color.h"

#define DFLT_BUMP_SCALE 0.1f

// sample height of current UV coord, then sample neighbor UV coords to get
// height difference.
// standard luminance formula (29.9% Red + 58.7% Green + 11.4% Blue)
// full wrapping: handle x > width, y > height, x < 0, y < 0
// static float	sample_bump_map(mlx_texture_t *bump, t_v2f uv, const t_v3f mod)
// {
// 	return (v3f_dot(sample_texture(bump, uv, mod),
// 			init_v3f(0.2126f, 0.7152f, 0.0722f)));
// }
// https://github.com/dorukb/Advanced-CPU-Raytracing/blob/master/src/hw6submission/sphere.cpp#L109
static t_v3f	apply_bump(t_bump *ctx, t_v2f uv, t_v3f mod)
{
	t_v2f	uv_u;
	t_v2f	uv_v;
	t_v3f	heights;
	t_v3f	height_deltas;
	float	bump_scale;

	uv_u = init_v2f(uv.u + ctx->delta, uv.v);
	uv_v = init_v2f(uv.u, uv.v + ctx->delta);

	(void)mod;
	// uv = v2f_mul_v3f(uv, mod);
	// uv_u = v2f_mul_v3f(uv_u, mod);
	// uv_v = v2f_mul_v3f(uv_v, mod);

	heights.x = v3f_dot(sample_noise(&uv, &ctx->mat->texture, g_v3f_zero), init_v3f(0.2126f, 0.7152f, 0.0722f));
	heights.y = v3f_dot(sample_noise(&uv_u, &ctx->mat->texture, g_v3f_zero), init_v3f(0.2126f, 0.7152f, 0.0722f));
	heights.z = v3f_dot(sample_noise(&uv_v, &ctx->mat->texture, g_v3f_zero), init_v3f(0.2126f, 0.7152f, 0.0722f));

	// heights.x = sample_bump_map(ctx->mat->bump_map, uv, mod);
	// heights.y = sample_bump_map(ctx->mat->bump_map, uv_u, mod);
	// heights.z = sample_bump_map(ctx->mat->bump_map, uv_v, mod);
	if (ctx->mat->bump_scale > 0.0f)
		bump_scale = ctx->mat->bump_scale;
	else
		bump_scale = DFLT_BUMP_SCALE;
	height_deltas = init_v3f(
			(heights.y - heights.x) * bump_scale / ctx->delta,
			(heights.z - heights.x) * bump_scale / ctx->delta,
			0.0f);

	t_v3f N = v3f_norm(v3f_cross(ctx->b, ctx->t));
	t_v3f par = v3f_scale(N, v3f_dot(height_deltas, N));
	t_v3f surf = v3f_sub(height_deltas, par);
	return (v3f_norm(v3f_sub(N, surf)));

	// ctx->p = v3f_add(ctx->n, v3f_scale(ctx->t, height_deltas.x));
	// ctx->p = v3f_add(ctx->p, v3f_scale(ctx->b, height_deltas.y));

	// ctx->p = v3f_cross(
	// 	v3f_add(ctx->t, v3f_scale(ctx->n, height_deltas.x)),
	// 	v3f_add(ctx->b, v3f_scale(ctx->n, height_deltas.y))
	// );

	return (v3f_norm(ctx->p));
}

static void	calc_tangent_basis(t_v3f n, t_v3f *t, t_v3f *b)
{
	t_v3f	up;

	if (fabs(v3f_dot(n, g_v3f_up)) > .99f)
		up = g_v3f_forward;
	else
		up = g_v3f_up;
	*t = v3f_norm(v3f_cross(up, n));
	*b = v3f_norm(v3f_cross(n, *t));
}

t_v3f	perturb_normal(const t_mat *mat, const t_v2f texcoord,
	const t_v3f normal)
{
	t_bump	ctx;

	ctx.n = normal;
	ctx.mat = mat;
	ctx.delta = 1.0f / mat->bump_map->width;

	calc_tangent_basis(normal, &ctx.t, &ctx.b);
	return (apply_bump(&ctx, texcoord, mat->texture.scale_rot));
}
