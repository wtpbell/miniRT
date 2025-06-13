/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   procedural_texturing.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/10 23:49:31 by jboon         #+#    #+#                 */
/*   Updated: 2025/06/13 10:17:03 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "rt_math.h"
#include "scene.h"

t_v3f	checkerboard_pattern(t_v2f texcoord, t_v3f obj_col, t_v3f alt_col)
{
	const float	scale_u = 10.0f;
	const float	scale_v = 10.0f;
	float		pattern;

	pattern = (modulo(texcoord.u * scale_u) < .5f) ^ (modulo(texcoord.v * scale_v) < .5f);
	return (v3f_lerp(obj_col, alt_col, pattern));
}
