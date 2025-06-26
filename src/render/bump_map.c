/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 09:15:29 by bewong            #+#    #+#             */
/*   Updated: 2025/06/26 21:30:36 by bewong           ###   ########.fr       */
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
static float	sample_bump_map(mlx_texture_t *bump_map, t_v2f uv)
{
	int		x;
	int		y;
	int		idx;
	float	rgb[3];

	uv.x = ft_clampf(uv.x, 0.0f, 1.0f - FLT_SML); 
	uv.y = ft_clampf(uv.y, 0.0f, 1.0f - FLT_SML);
	x = (int)(uv.x * (bump_map->width - 1)); //  convert uv to pixel coords
	y = (int)(uv.y * (bump_map->height - 1));
	x = x % bump_map->width; // texture wrapping (tiling), e.g. x = 300 → 300 % 256 = 44 
	y = y % bump_map->height;
	if (x < 0) // handle negative coords
		x += bump_map->width; // if x = -5, width = 256 => x = 251
	if (y < 0)
		y += bump_map->height;
	idx = (y * bump_map->width + x) * 4; // pixel(10,20) => idx = (20 * 256 + 10) * 4 = 5124, 2D->1D pixel array
	rgb[0] = bump_map->pixels[idx] / 255.0f;
	rgb[1] = bump_map->pixels[idx + 1] / 255.0f;
	rgb[2] = bump_map->pixels[idx + 2] / 255.0f;
	return (0.2126f * rgb[0] + 0.7152f * rgb[1] + 0.0722f * rgb[2]); //standard luminance formula, grayscale intensity
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
	uv_u = (t_v2f){.x = uv.x + ctx->delta, .y = uv.y}; // sample right neighbor
	uv_v = (t_v2f){.x = uv.x, .y = uv.y + ctx->delta}; // sample top neighbor
	heights.x = sample_bump_map(ctx->mat->bump_map, uv); // get bump height (center)
	heights.y = sample_bump_map(ctx->mat->bump_map, uv_u); // get bump height (right)
	heights.z = sample_bump_map(ctx->mat->bump_map, uv_v); // get bump height (top)
	if (ctx->mat->bump_scale > 0.0f)
		bump_scale = ctx->mat->bump_scale;
	else
		bump_scale = DFLT_BUMP_SCALE;
	height_deltas = (t_v3f){ // calculate the surface slopes in the tangent space
		.x = (heights.y - heights.x) * bump_scale, //du
		.y = (heights.z - heights.x) * bump_scale,	//dv
		.z = 0.0f
	};
	ctx->p = v3f_add(ctx->n, v3f_scale(ctx->t, height_deltas.x)); //move (tangent) on U slope
	ctx->p = v3f_add(ctx->p, v3f_scale(ctx->b, height_deltas.y));//move (bitangent) on V slope
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
