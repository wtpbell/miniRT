/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   procedural_texturing.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/10 23:49:31 by jboon         #+#    #+#                 */
/*   Updated: 2025/07/31 19:29:53 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "rt_math.h"
#include "scene.h"

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

t_v3f	noise_pattern(const t_v2f *texcoord, const t_tex *tex, t_v3f col_a)
{
	t_v2f	point;

	point = v2f_mul_v3f(*texcoord, tex->scale_rot);
	return (v3f_lerp(
			col_a, tex->col, (tex->fp_perlin(point, &tex->p_data) + 1.0f)
			* 0.5f));
}
