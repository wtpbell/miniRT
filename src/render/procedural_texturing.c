/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   procedural_texturing.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/10 23:49:31 by jboon         #+#    #+#                 */
/*   Updated: 2025/06/27 15:46:01 by jboon         ########   odam.nl         */
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
	(void)col_a;
	return (sample_texture(tex->tex, *texcoord, tex->scale_rot));
}
