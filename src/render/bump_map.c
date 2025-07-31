/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   bump_map.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/24 09:15:29 by bewong        #+#    #+#                 */
/*   Updated: 2025/07/31 16:20:47 by jboon         ########   odam.nl         */
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
static t_v3f	sample_heights(t_bump_ctx *ctx, t_v2f uv, t_v3f mod)
{
	const t_v3f	gray = init_v3f(0.2126f, 0.7152f, 0.0722f);
	const float	size = ctx->size;
	t_v2f		uv_u;
	t_v2f		uv_v;

	uv_u = init_v2f(uv.u + ctx->delta.u, uv.v);
	uv_v = init_v2f(uv.u, uv.v + ctx->delta.v);
	if (ctx->type == TEX_IMAGE)
	{
		return (init_v3f(
				v3f_dot(sample_texture(ctx->tex, uv, mod), gray),
				v3f_dot(sample_texture(ctx->tex, uv_u, mod), gray),
				v3f_dot(sample_texture(ctx->tex, uv_v, mod), gray)
			));
	}
	return (init_v3f(
			((ctx->fp_perlin(v2f_mul_v3f(uv, mod), ctx->p_data) + 1.0f)
				* 0.5f) / size,
			((ctx->fp_perlin(v2f_mul_v3f(uv_u, mod), ctx->p_data) + 1.0f)
				* 0.5f) / size,
			((ctx->fp_perlin(v2f_mul_v3f(uv_v, mod), ctx->p_data) + 1.0f)
				* 0.5f) / size
		));
}

static t_v3f	apply_bump(t_bump_ctx *ctx, t_v2f uv, t_v3f mod)
{
	t_v3f	heights;
	t_v3f	height_deltas;
	t_v3f	perp_norm;

	heights = sample_heights(ctx, uv, mod);
	height_deltas = init_v3f((heights.y - heights.x) * ctx->scale,
			(heights.z - heights.x) * ctx->scale,
			0.0f);
	perp_norm = v3f_add(ctx->n, v3f_scale(ctx->t, height_deltas.x));
	perp_norm = v3f_add(perp_norm, v3f_scale(ctx->b, height_deltas.y));
	return (v3f_norm(perp_norm));
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
	const float	perl_size = 1.0f / 256.0f;
	t_bump_ctx	ctx;

	ctx.n = normal;
	ctx.type = mat->bump_map.type;
	if (ctx.type == TEX_PERLIN)
	{
		ctx.delta = init_v2f(perl_size, perl_size);
		ctx.fp_perlin = mat->bump_map.fp_perlin;
	}
	else
	{
		ctx.delta = init_v2f(1.0f / mat->bump_map.tex->width,
				1.0f / mat->bump_map.tex->height);
		ctx.tex = mat->bump_map.tex;
	}
	ctx.size = v2f_mag(ctx.delta);
	ctx.p_data = &mat->bump_map.p_data;
	if (mat->bump_map.scale > 0.0f)
		ctx.scale = mat->bump_map.scale;
	else
		ctx.scale = DFLT_BUMP_SCALE;
	calc_tangent_basis(normal, &ctx.t, &ctx.b);
	return (apply_bump(&ctx, texcoord, mat->texture.scale_rot));
}
