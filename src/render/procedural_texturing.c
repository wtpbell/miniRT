/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   procedural_texturing.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/10 23:49:31 by jboon         #+#    #+#                 */
/*   Updated: 2025/06/16 18:58:25 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "rt_math.h"
#include "scene.h"

t_v3f	checkerboard_pattern(t_v2f texcoord, t_v2f scale, t_v3f prim_col, t_v3f alt_col)
{
	float	pattern;

	pattern = (modulo(texcoord.u * scale.u) < .5f) ^ (modulo(texcoord.v * scale.v) < .5f);
	return (v3f_lerp(prim_col, alt_col, pattern));
}
