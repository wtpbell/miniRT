/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   procedural_texturing.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 23:49:31 by jboon             #+#    #+#             */
/*   Updated: 2025/06/26 17:04:15 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_math.h"
#include "scene.h"
#include "color.h"
#include <stdio.h>

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
	u = ft_clampf01(texcoord->u * tex->scale_rot.u
			- floorf(texcoord->u * tex->scale_rot.u));
	v = ft_clampf01(texcoord->v * tex->scale_rot.v
			- floorf(texcoord->v * tex->scale_rot.v));
	x = (t_col32)(u * (tex->tex->width - 1));
	y = (t_col32)(v * (tex->tex->height - 1));
	idx = (y * tex->tex->width + x) * tex->tex->bytes_per_pixel;
	return (col32_to_v3f(init_col32(
				tex->tex->pixels[idx],
				tex->tex->pixels[idx + 1],
				tex->tex->pixels[idx + 2],
				tex->tex->pixels[idx + 3]
			)));
}
