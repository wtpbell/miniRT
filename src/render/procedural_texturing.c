/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   procedural_texturing.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/10 23:49:31 by jboon         #+#    #+#                 */
/*   Updated: 2025/06/18 14:19:35 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "rt_math.h"
#include "scene.h"

t_v3f	solid_pattern(const t_v2f *texcoord, const t_tex *tex, t_v3f prim_col)
{
	(void)texcoord;
	(void)tex;
	return (prim_col);
}

t_v3f	checker_pattern(const t_v2f *texcoord, const t_tex *tex, t_v3f prim_col)
{
	float	pattern;

	pattern = (modulo(texcoord->u * tex->scale_uv.u) < .5f)
		^ (modulo(texcoord->v * tex->scale_uv.v) < .5f);
	return (v3f_lerp(prim_col, tex->col, pattern));
}

// t_v3f	checker_pattern(t_v2f texcoord, t_v2f size, t_v3f col_a, t_v3f col_b)
// {
// 	float	u;
// 	float	v;

// 	u = floorf(texcoord.u * size.x);
// 	v = floorf(texcoord.y * size.y);

// 	if (fmodf(u + v, 2.0f) == 0.0f)
// 		return (col_a);
// 	return (col_b);
// }
