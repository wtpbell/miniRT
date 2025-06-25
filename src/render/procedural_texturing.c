/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   procedural_texturing.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 23:49:31 by jboon             #+#    #+#             */
/*   Updated: 2025/06/25 20:12:23 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_math.h"
#include "scene.h"
#include "color.h"

t_v3f	solid_pattern(const t_v2f *texcoord, const t_tex *tex, t_v3f col_a)
{
	(void)texcoord;
	(void)tex;
	return (col_a);
}

t_v3f	checker_pattern(const t_v2f *texcoord, const t_tex *tex, t_v3f col_a)
{
	float	pattern;

	pattern = (modulo(texcoord->u * tex->scale_rot.u) < .5f)
		^ (modulo(texcoord->v * tex->scale_rot.v) < .5f);
	return (v3f_lerp(col_a, tex->col, pattern));
}

t_v3f	image_pattern(const t_v2f *texcoord, const t_tex *tex, t_v3f col_a)
{
	float	u;
	float	v;
	t_col32	x;
	t_col32	y;
	t_col32	idx;

	(void)col_a;
	u = fmodf(texcoord->u * tex->scale_rot.u, 1.0f);
	v = fmodf(texcoord->v * tex->scale_rot.v, 1.0f);
	if (u < 0)
		u += 1.0f;
	if (v < 0)
		v += 1.0f;
	x = (t_col32)(u * (tex->tex->width - 1));
	y = (t_col32)(v * (tex->tex->height - 1));
	idx = (y * tex->tex->width + x) * tex->tex->bytes_per_pixel;;
	return (col32_to_v3f(init_col32(
		tex->tex->pixels[idx],
		tex->tex->pixels[idx + 1],
		tex->tex->pixels[idx + 2],
		tex->tex->pixels[idx + 3]
	)));
}
