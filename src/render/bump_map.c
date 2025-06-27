/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   bump_map.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/24 09:15:29 by bewong        #+#    #+#                 */
/*   Updated: 2025/06/27 15:46:25 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "rt_math.h"
#include "scene.h"
#include "color.h"

// https://learnopengl.com/Advanced-Lighting/Normal-Mapping
// https://learnopengl.com/Advanced-Lighting/Normal-Mapping
// https://dev.to/ndesmic/webgl-engine-from-scratch-14-bump-maps-and-simple-animation-3fef
// https://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/
//Vec3 normal = Vec3(2*(center_r-center_l), 2*(center_d-center_u), -4).Normalize();

#define DFLT_BUMP_SCALE 0.1f

// sample height of current UV coord, then sample neighbor UV coords to get height difference
// standard luminance formula (29.9% Red + 58.7% Green + 11.4% Blue)
// UV(0.5, 0.5) -> (127, 127) UV[0,1] -> pixels[0,width - 1] / pixels[0,height - 1]
// full wrapping: handle x > width, y > height, x < 0, y < 0
static float	sample_bump_map(mlx_texture_t *bump, t_v2f uv, const t_v3f mod)
{
	return (v3f_dot(
		sample_texture(bump, uv, mod),
			init_v3f(0.2126f, 0.7152f, 0.0722f)));
}

static t_v3f	apply_bump(t_bump *ctx, t_v2f uv, t_v3f mod)
{
	t_v2f	uv_u;
	t_v2f	uv_v;
	t_v3f	heights;
	t_v3f	height_deltas;
	float	bump_scale;

	// sample neighbors right and up
	uv_u = init_v2f(uv.u + ctx->delta, uv.v);
	uv_v = init_v2f(uv.u, uv.v + ctx->delta); // what about the aspect ratio

	// Sample the bump map at three locations
	heights.x = sample_bump_map(ctx->mat->bump_map, uv, mod);
	heights.y = sample_bump_map(ctx->mat->bump_map, uv_u, mod);
	heights.z = sample_bump_map(ctx->mat->bump_map, uv_v, mod);

	if (ctx->mat->bump_scale > 0.0f)
		bump_scale = ctx->mat->bump_scale;
	else
		bump_scale = DFLT_BUMP_SCALE;

	// calculate the surface slopes in the tangent space
	height_deltas = init_v3f(
		(heights.y - heights.x) * bump_scale,
		(heights.z - heights.x) * bump_scale,
		0.0f
	);

	ctx->p = v3f_add(ctx->n, v3f_scale(ctx->t, height_deltas.x)); //move (tangent) on U slope
	ctx->p = v3f_add(ctx->p, v3f_scale(ctx->b, height_deltas.y)); //move (bitangent) on V slope
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

t_v3f	perturb_normal(const t_mat *mat, const t_v2f texcoord, const t_v3f normal)
{
	t_bump	ctx;

	ctx.n = normal;
	ctx.mat = mat;
	ctx.delta = 1.0f / mat->bump_map->width;
	calc_tangent_basis(normal, &ctx.t, &ctx.b);
	return (apply_bump(&ctx, texcoord, mat->texture.scale_rot));
}
