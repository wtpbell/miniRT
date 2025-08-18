/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   vector_init.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/08 19:35:46 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/15 10:52:41 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

const t_v3f	g_v3f_zero = (t_v3f){.x = 0.0f, .y = 0.0f, .z = 0.0f};
const t_v3f	g_v3f_one = (t_v3f){.x = 1.0f, .y = 1.0f, .z = 1.0f};
const t_v3f	g_v3f_forward = (t_v3f){.x = 0.0f, .y = 0.0f, .z = 1.0f};
const t_v3f	g_v3f_back = (t_v3f){.x = 0.0f, .y = 0.0f, .z = -1.0f};
const t_v3f	g_v3f_up = (t_v3f){.x = 0.0f, .y = 1.0f, .z = 0.0f};
const t_v3f	g_v3f_down = (t_v3f){.x = 0.0f, .y = -1.0f, .z = 0.0f};
const t_v3f	g_v3f_right = (t_v3f){.x = 1.0f, .y = 0.0f, .z = 0.0f};
const t_v3f	g_v3f_left = (t_v3f){.x = -1.0f, .y = 0.0f, .z = 0.0f};
const t_v2f	g_v2f_zero = (t_v2f){.x = 0.0f, .y = 0.0f};
const t_v2f	g_v2f_one = (t_v2f){.x = 1.0f, .y = 1.0f};

t_v2f	init_v2f(float x, float y)
{
	return ((t_v2f){
		.x = x,
		.y = y
	});
}

t_v2i	init_v2i(int x, int y)
{
	return ((t_v2i){
		.x = x,
		.y = y
	});
}

t_v3f	init_v3f(float x, float y, float z)
{
	return ((t_v3f){
		.x = x,
		.y = y,
		.z = z
	});
}

t_v4f	init_v4f(float x, float y, float z, float w)
{
	return ((t_v4f){
		.x = x,
		.y = y,
		.z = z,
		.w = w
	});
}
